# -*- mode: python ; coding: utf-8 -*-
import glob
import os

block_cipher = None

# Funktion zum Ermitteln aller Python-Module im src-Verzeichnis
def find_modules(path):
    modules = []
    for root, _, files in os.walk(path):
        for file in files:
            if file.endswith('.py') and file != '__init__.py':
                module = os.path.relpath(os.path.join(root, file), path)
                module = module.replace(os.sep, '.')[:-3]  # Entferne '.py'
                modules.append(module)
    return modules

# Finde alle Module im src-Verzeichnis
hiddenimports = find_modules('src')

a = Analysis(
    ['src/main.py'],
    pathex=['.', './src'],
    binaries=[],
    datas=[],
    hiddenimports=hiddenimports,  # Automatisch ermittelte Module
    hookspath=[],
    runtime_hooks=[],
    excludes=[],
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=block_cipher,
    noarchive=False,
)

pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    [],
    exclude_binaries=True,
    name='main',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    console=True,
)

coll = COLLECT(
    exe,
    a.binaries,
    a.zipfiles,
    a.datas,
    strip=False,
    upx=True,
    upx_exclude=[],
    name='main',
)
