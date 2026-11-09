// Windows-side GUI test: does what Equalizer APO's "Open panel" does (effEditGetRect, effEditOpen into a
// parent window, effEditGetRect again), runs the idle loop, sends mouse clicks, and closes the editor.
#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <cstring>
struct AEffect;
typedef intptr_t (*audioMasterCallback)(AEffect*, int32_t, int32_t, intptr_t, void*, float);
struct AEffect { int32_t magic; intptr_t (*dispatcher)(AEffect*, int32_t, int32_t, intptr_t, void*, float); void* a; void (*setParameter)(AEffect*, int32_t, float); float (*getParameter)(AEffect*, int32_t); int32_t numPrograms, numParams, numInputs, numOutputs, flags; intptr_t r1, r2; int32_t id, rq, oq; float io; void* obj; void* user; int32_t uid, ver; void (*pr)(AEffect*, float**, float**, int32_t); };
struct VstRect { int16_t top, left, bottom, right; };
static int g_automate = 0;
static intptr_t master(AEffect*, int32_t op, int32_t idx, intptr_t, void*, float) { if (op == 0) ++g_automate; return op == 1 ? 2400 : 0; }
static LRESULT CALLBACK wp(HWND h, UINT m, WPARAM w, LPARAM l) { return DefWindowProcA(h, m, w, l); }
static HWND g_child = nullptr;
static BOOL CALLBACK enumChild(HWND h, LPARAM) { g_child = h; return FALSE; }
static void pump(AEffect* e, int ms) { DWORD t0 = GetTickCount(); MSG msg; while (GetTickCount() - t0 < (DWORD)ms) { while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) { TranslateMessage(&msg); DispatchMessageA(&msg); } e->dispatcher(e, 19, 0, 0, 0, 0); Sleep(10); } }
static void click(int x, int y) { LPARAM lp = MAKELPARAM(x, y); PostMessageA(g_child, WM_MOUSEMOVE, 0, lp); PostMessageA(g_child, WM_LBUTTONDOWN, MK_LBUTTON, lp); PostMessageA(g_child, WM_LBUTTONUP, 0, lp); }
static void dragTo(int x0, int y, int x1) { PostMessageA(g_child, WM_MOUSEMOVE, 0, MAKELPARAM(x0, y)); PostMessageA(g_child, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x0, y)); for (int x = x0; x <= x1; x += 10) PostMessageA(g_child, WM_MOUSEMOVE, MK_LBUTTON, MAKELPARAM(x, y)); PostMessageA(g_child, WM_LBUTTONUP, 0, MAKELPARAM(x1, y)); }
int main(int, char** argv) {
  HMODULE h = LoadLibraryA(argv[1]); if (!h) { printf("LoadLibrary fail %lu\n", GetLastError()); return 1; }
  typedef AEffect* (*M)(audioMasterCallback); M m = (M)GetProcAddress(h, "VSTPluginMain"); AEffect* e = m(master);
  auto d = [&](int op, int idx = 0, intptr_t v = 0, void* p = nullptr, float f = 0) { return e->dispatcher(e, op, idx, v, p, f); };
  d(0); d(10, 0, 0, nullptr, 48000.f); d(11, 0, 512);
  printf("flags 0x%x hasEditor=%d\n", e->flags, e->flags & 1);
  WNDCLASSA wc = {}; wc.lpfnWndProc = wp; wc.hInstance = GetModuleHandleA(0); wc.lpszClassName = "host"; RegisterClassA(&wc);
  HWND parent = CreateWindowA("host", "ArcadeAmp", WS_POPUP | WS_VISIBLE, 20, 20, 560, 540, 0, 0, wc.hInstance, 0);
  VstRect* r = nullptr; d(13, 0, 0, &r);
  printf("GetRect before open: %s\n", r ? "pointer set" : "NULL (this is what crashed Editor.exe)");
  intptr_t ok = d(14, 0, 0, parent);
  r = nullptr; d(13, 0, 0, &r);
  if (!r) { puts("FAIL: no rect"); return 2; }
  printf("editOpen=%ld rect %d x %d\n", (long)ok, r->right - r->left, r->bottom - r->top);
  SetWindowPos(parent, 0, 20, 20, r->right - r->left, r->bottom - r->top, SWP_NOZORDER);
  ShowWindow(parent, SW_SHOW);
  pump(e, 3000);
  EnumChildWindows(parent, enumChild, 0); printf("child window %p\n", (void*)g_child);
  
  // click "Heavy" preset button (index 3). layout: margin 24, gap 8, width (560-48-40)/6
  float pw = (560 - 48 - 40) / 6.0f; click((int)(24 + 3 * (pw + 8) + pw / 2), 112 + 17); pump(e, 6000);
  printf("after clicking Heavy: preset value %.2f (3/5 = 0.60), automate calls %d\n", e->getParameter(e, 0), g_automate);
  
  // drag the Hum slider (row 4 -> y = 206 + 3*34) from 150 to 300
  dragTo(160, 206 + 3 * 34, 300); pump(e, 6000);
  printf("after dragging Hum: preset %.2f, drive %.2f hum %.2f (Heavy copied: drive .25, hum was .25 then dragged)\n", e->getParameter(e, 0), e->getParameter(e, 1), e->getParameter(e, 4));
  click(150 + 88 + 30, 206 + 7 * 34 + 8 + 15); pump(e, 6000);
  printf("after clicking 50 Hz: mains %.2f\n", e->getParameter(e, 8));
  
  d(15); puts("editClose ok"); d(1); puts("done");
  return 0; }
