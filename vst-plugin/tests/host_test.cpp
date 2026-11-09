// Minimal VST2 host: loads the REAL plugin binary (Linux build of the same wrapper),
// lists parameters as a host shows them, sets values through the VST2 API, processes audio.
#include <dlfcn.h>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>
#include <cstdint>
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
int main(int, char** argv) {
  void* h = dlopen(argv[1], RTLD_NOW); if (!h) { printf("dlopen fail %s\n", dlerror()); return 1; }
  typedef AEffect* (*Main)(audioMasterCallback); Main m = (Main)dlsym(h, "VSTPluginMain");
  AEffect* e = m(master); if (!e) { puts("null"); return 1; }
  auto d = [&](int op, int idx = 0, intptr_t v = 0, void* p = nullptr, float f = 0) { return e->dispatcher(e, op, idx, v, p, f); };
  d(0); float sr = 48000; d(10, 0, 0, nullptr, sr); d(11, 0, 512); d(12, 0, 1);
  printf("params: %d, in %d out %d\n", e->numParams, e->numInputs, e->numOutputs);
  for (int i = 0; i < e->numParams; ++i) { char n[64] = {}, s[64] = {}; d(8, i, 0, n); d(7, i, 0, s);
    printf("  [%d] %-10s value %.3f display '%s'\n", i, n, e->getParameter(e, i), s); }
  // Parameter properties (opcode 56) read with the Steinberg VstParameterProperties layout, as a
  // real host does. A host that builds an integer slider from min/max must never see max <= min.
  for (int i = 0; i < e->numParams; ++i) {
    alignas(8) unsigned char raw[512] = {};
    d(56, i, 0, raw);
    int32_t flags; memcpy(&flags, raw + 76, 4);
    int16_t mn, mx; memcpy(&mn, raw + 80, 2); memcpy(&mx, raw + 82, 2);
    bool bad = (flags & 2) && mx <= mn;
    printf("  props[%d] flags=0x%x intLimits=%s%s\n", i, flags, (flags & 2) ? "yes" : "no", bad ? "  <-- BAD: host would see min>=max" : "");
    if (bad) { puts("FAIL: integer limits unreadable by host"); return 2; }
  }
  // preset selector display
  for (int p = 0; p < 6; ++p) { e->setParameter(e, 0, p / 5.0f); char s[64] = {}; d(7, 0, 0, s); printf("  preset %.2f -> '%s'\n", p / 5.0f, s); }
  auto run = [&](const char* label, std::vector<std::pair<int,float>> sets, bool silence) {
    for (auto& kv : sets) e->setParameter(e, kv.first, kv.second);
    std::vector<float> in(48000), L(48000), R(48000);
    for (int i = 0; i < 48000; ++i) in[i] = silence ? 0.f : 0.3f * std::sin(2 * M_PI * 440 * i / 48000.0);
    float* ip[2] = { in.data(), in.data() }; float* op[2] = { L.data(), R.data() };
    for (int b = 0; b < 48000; b += 512) { float* ib[2] = { ip[0] + b, ip[1] + b }; float* ob[2] = { op[0] + b, op[1] + b }; int n = b + 512 <= 48000 ? 512 : 48000 - b; e->processReplacing(e, ib, ob, n); }
    double a = 0, bq = 0, c = 0; for (int i = 24000; i < 48000; ++i) { a += L[i] * L[i]; bq += R[i] * R[i]; c += in[i] * in[i]; }
    printf("  %-34s L %.1f dBFS  R %.1f dBFS  (input %.1f)\n", label, 10 * log10(a / 24000 + 1e-20), 10 * log10(bq / 24000 + 1e-20), 10 * log10(c / 24000 + 1e-20));
  };
  // normalized values: preset/5, percent/100
  run("defaults, 0.3 tone", { {0, 0.f} }, false);
  run("Custom: Drive 0", { {1, 0.f} }, false);
  run("Preset Medium (2/5), tone", { {0, 0.4f} }, false);
  run("Preset Heavy (3/5), tone", { {0, 0.6f} }, false);
  run("Preset Medium, silence", { {0, 0.4f} }, true);
  run("Preset TV early, silence", { {0, 0.8f} }, true);
  run("Custom Hum 25 only, silence", { {0, 0.f}, {1, 0.f}, {4, 0.25f} }, true);
  return 0; }
