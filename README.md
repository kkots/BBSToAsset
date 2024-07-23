# BBSToAsset

Converter between .bbscript format, as produced by ggxrd-mod (<https://github.com/super-continent/ggxrd-mod>) and bbscript parser/rebuilder (<https://github.com/super-continent/bbscript>), and the .REDAssetCharaScript file extension format, as produced by gildor's extract tool (<https://www.gildor.org/downloads>) when unpackaging a XXX_DAT_SF.upk file (for example, DZY_DAT_SF.upk) from Guilty Gear Xrd Rev2 REDGame\CookedPCConsole directory (after the necessary decryption (<https://github.com/gdkchan/GGXrdRevelatorDec>) and decompression (<https://www.gildor.org/downloads>) operations).

##Usage

```txt
BBSToAsset BBSCRFILE ORIGINAL_REDASSET NEW_REDASSET
```

, where

- **BBSCRFILE** - the .bbscript file to convert to a new .REDAssetCharaScript file,
- **ORIGINAL_REDASSET** - the original REDAssetCharaScript file from which you originally obtained the bbscript,
- **NEW_REDASSET** - where to create the new .REDAssetCharaScript file

```txt
AssetToBBS REDASSET NEWBBSCR
```

, where

- **REDASSET** - the .REDAssetCharaScript file to convert to a .bbscript file,
- **NEWBBSCR** - where to create the new .bbscript file

## Building/running

Only runs on Windows. Should be easy to make work on Linux by replacing all file operation code.  
To build, open this project in Visual Studio, press Build - Build Solution. It should create two exes - BBSToAsset and AssetToBBS, and say in the Output panel where they got created.
