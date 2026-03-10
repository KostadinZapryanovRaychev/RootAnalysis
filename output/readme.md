# Muogram Analysis — 2D Muography Comparison Macro

```bash
cd CMSSW_16_0_1/src/DQM/RPCMonitorModule/macros/Muogram
root -l -b -q 'muogr_v2.cxx("/path/to/fileRef.root", "/path/to/fileComp.root", "Muogr", "2018", "2025", "rollNames.txt")'
```

### EOS example (lxplus)

```bash
root -l -b -q 'muogr_v2.cxx("/eos/cms/store/group/dpg_rpc/comm_rpc/Sandbox/mileva/AnalyzeEfficiency_321475_325172_RPCMon_2018D.root", "/eos/cms/store/group/dpg_rpc/comm_rpc/Sandbox/mileva/AnalyzeEfficiency_396805_397817_RPCMon2025F.root", "Muogr", "2018", "2025", "rollNames.txt")'
```

### rollNames.txt format

```
# one roll name per line, lines starting with # are ignored
Muography_W-2_RB1in_S07_Backward
Muography_W-2_RB1in_S07_Forward
Muography_W-2_RB1out_S07_Backward
...
```

### Output

All files are saved into `Muogram/output/`:

- `<roll>_<year1>.png` — reference muography map
- `<roll>_<year2>.png` — comparison muography map
- `<roll>_<year1>vs<year2>_relDiff1D.png/.C` — relative difference with Gaussian fit
