import qbs 1.0

TiledPlugin {
    cpp.defines: ["DOFUS_LIBRARY"]

    files: [
        "dofus_global.h",
        "dofusplugin.cpp",
        "dofusplugin.h",
    ]
}
