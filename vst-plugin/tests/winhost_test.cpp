// Minimal VST2 host: loads the REAL plugin binary (Linux build of the same wrapper),
// lists parameters as a host shows them, sets values through the VST2 API, processes audio.
#include <windows.h>
#include <cstdio>
#include <cmath>
#include <vector>
#include <cstdint>
#include <cstring>
struct AEffect;
typedef intptr_t (*audioMasterCallback)(AEffect*, int32_t, int32_t, intptr_t, void*, float);
struct AEffect {
  int32_t magic;
  intptr_t (*dispatcher)(AEffect*, int32_t, int32_t, intptr_t, void*, float);
  void (*process)(AEffect*, float**, float**, int32_t);
  void (*setParameter)(AEffect*, int32_t, float);
  float (*getParameter)(AEffect*, int32_t);
  int32_t numPrograms, numParams, numInputs, numOutputs, flags;
  intptr_t resvd1, resvd2;
  int32_t initialDelay, realQualities, offQualities;
  float ioRatio;
  void* object; void* user;
  int32_t uniqueID, version;
  void (*processReplacing)(AEffect*, float**, float**, int32_t);
};
static intptr_t master(AEffect*, int32_t op, int32_t, intptr_t, void*, float) { return op == 1 ? 2400 : 0; }

struct VstPP { float stepFloat, smallStepFloat, largeStepFloat; char label[64]; int32_t flags; int16_t minInteger, maxInteger, stepInteger, largeStepInteger; char shortLabel[8]; };
int main(int, char** argv) {
  HMODULE h = LoadLibraryA(argv[1]); if (!h) { printf("LoadLibrary fail %lu\n", GetLastError()); return 1; }
  typedef AEffect* (*Main)(audioMasterCallback); Main m = (Main)GetProcAddress(h, "VSTPluginMain"); if(!m) m=(Main)GetProcAddress(h,"main");
  AEffect* e = m(master); if (!e) { puts("null"); return 1; }
  auto d = [&](int op, int idx = 0, intptr_t v = 0, void* p = nullptr, float f = 0) { return e->dispatcher(e, op, idx, v, p, f); };
  d(0); float sr = 48000; d(10, 0, 0, nullptr, sr); d(11, 0, 512); d(12, 0, 1);
  printf("magic %x params %d flags %x\n", e->magic, e->numParams, e->flags);
  char buf[256] = {};
  d(45,0,0,buf); printf("effect name '%s'\n", buf); d(47,0,0,buf); printf("vendor '%s'\n", buf); d(48,0,0,buf); printf("product '%s'\n", buf);
  printf("vst version %ld, category %ld, hasEditor flag %d\n", (long)d(58), (long)d(35), (e->flags & 1));
  for (int i = 0; i < e->numParams; ++i) {
    char n[256]={}, l[256]={}, s[256]={}; d(8,i,0,n); d(6,i,0,l); d(7,i,0,s);
    alignas(8) unsigned char raw[512]; memset(raw,0,512); intptr_t r56 = d(56,i,0,raw);
    int32_t fl; memcpy(&fl, raw+76, 4); int16_t mn,mx; memcpy(&mn,raw+80,2); memcpy(&mx,raw+82,2);
    printf("[%d] '%s' label '%s' disp '%s' automatable=%ld props=%ld flags=%x intmin=%d intmax=%d\n", i,n,l,s,(long)d(26,i),(long)r56,fl,mn,mx);
    for (int k = 0; k <= 10; ++k) { e->setParameter(e, i, k/10.0f); char t[256]={}; d(7,i,0,t); float g = e->getParameter(e,i); (void)g; }
    char sv[] = "5"; d(27,i,0,sv);
  }
  float* in0 = new float[512](), *in1 = new float[512](), *o0 = new float[512](), *o1 = new float[512]();
  float* ip[2] = {in0,in1}; float* op[2] = {o0,o1};
  for (int p = 0; p < 6; ++p) { e->setParameter(e,0,p/5.0f); for (int k=0;k<20;++k) e->processReplacing(e,ip,op,512); }
  float* ipx[2]={in0,in1}; e->process(e, ipx, op, 512);   // accumulate/old path
  d(12,0,0); d(1); puts("closed ok");
  return 0; }
