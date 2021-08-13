import hashlib

EMD_PATH="QuickMenuReborn.emd"
YML_PATH="QuickMenuReborn.yml"

EMD_START="Module: QuickMenuReborn major_version: 3 minor_version: 0\nsceModuleInfo_type: 6\nmodule_function: module_start localname: module_start\nmodule_function: module_stop localname: module_stop\n\nLibrary: QuickMenuReborn version: 1\nLibrary: QuickMenuReborn nidsuffix: \"\"\nLibrary: QuickMenuReborn attr: auto_export\n"
LIB_NAME="QuickMenuReborn"
functionNames=[ 
        "QuickMenuRebornAddWidget",
        "QuickMenuRebornRemoveWidget",
        "QuickMenuRebornUpdateWidget",
        "makeWidgetColor",
        "makeWidgetVector4",
        "QuickMenuRebornButton",
        "QuickMenuRebornCheckBox",
        "QuickMenuRebornText",
        "QuickMenuRebornPlane",
        "QuickMenuRebornSeparator",
        "QuickMenuRebornUpdateButton",
        "QuickMenuRebornUpdateCheckBox",
        "QuickMenuRebornUpdateText",
        "QuickMenuRebornUpdatePlane",
        "QuickMenuRebornRemoveSeparator",
        "QuickMenuRebornGetCheckBoxState"
]

def getHash(string):
    return str(hashlib.sha256(string.encode('utf-8')).hexdigest())[:8].upper()

def writeEMD():
    print("Writing EMD")
    with open(EMD_PATH, 'wt') as f:
        print("EMD: Writing initial block")
        f.write(EMD_START)
        
        print("EMD: Writing libnamenid")
        f.write("Library: " + LIB_NAME + " libnamenid: 0x" + getHash(LIB_NAME) + "\n")    
        
        for i in functionNames:
            print("EMD: Adding function " + i)
            f.write("Library: " + LIB_NAME + " function: " + i + " nidvalue: 0x" + getHash(i) + "\n")


def WriteYML():
    print("Writing YML")
    with open(YML_PATH, 'wt') as f:
        print("YML: Writing Initial block")
        f.write("modules:\n  QuickMenuReborn:\n    libraries:\n      " + LIB_NAME + ":\n        nid: 0x" + getHash(LIB_NAME) + "\n        functions:\n")
        for i in functionNames:
            print("YML: Adding function " + i)
            f.write("          " + i + ": 0x" + getHash(i) + "\n")

writeEMD()
WriteYML()