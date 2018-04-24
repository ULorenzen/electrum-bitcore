#!/usr/bin/env python3

# python setup.py sdist --format=zip,gztar

from setuptools import setup
from distutils.core import Extension
import os
import sys
import platform
import imp
import argparse

with open('contrib/requirements/requirements.txt') as f:
    requirements = f.read().splitlines()

with open('contrib/requirements/requirements-hw.txt') as f:
    requirements_hw = f.read().splitlines()

version = imp.load_source('version', 'lib/version.py')

if sys.version_info[:3] < (3, 4, 0):
    sys.exit("Error: Electrum requires Python version >= 3.4.0...")

data_files = ['contrib/requirements/' + r for r in ['requirements.txt', 'requirements-hw.txt']]

if platform.system() in ['Linux', 'FreeBSD', 'DragonFly']:
    parser = argparse.ArgumentParser()
    parser.add_argument('--root=', dest='root_path', metavar='dir', default='/')
    opts, _ = parser.parse_known_args(sys.argv[1:])
    usr_share = os.path.join(sys.prefix, "share")
    if not os.access(opts.root_path + usr_share, os.W_OK) and \
       not os.access(opts.root_path, os.W_OK):
        if 'XDG_DATA_HOME' in os.environ.keys():
            usr_share = os.environ['XDG_DATA_HOME']
        else:
            usr_share = os.path.expanduser('~/.local/share')
    data_files += [
        (os.path.join(usr_share, 'applications/'), ['electrum.desktop']),
        (os.path.join(usr_share, 'pixmaps/'), ['icons/electrumBTX.png'])
    ]
ext = Extension('_powhash',
                 define_macros = [('MAJOR_VERSION', '1'),
                                  ('MINOR_VERSION', '0')],
                 include_dirs = ['/usr/local/include',
                                 'btxcext/inc'],
                 sources = ['btxcext/_powhash.cpp',
                            'btxcext/aes_helper.c',
                            'btxcext/blake.c',
                            'btxcext/bmw.c',
                            'btxcext/groestl.c',
                            'btxcext/jh.c',
                            'btxcext/keccak.c',
                            'btxcext/skein.c',
                            'btxcext/luffa.c',
                            'btxcext/cubehash.c',
                            'btxcext/shavite.c',
                            'btxcext/simd.c',
                            'btxcext/echo.c'])

setup(
    name="Electrum-BitCore",
    version=version.ELECTRUM_VERSION,
    install_requires=requirements,
    extras_require={
        'hardware': requirements_hw,
    },
    packages=[
        'electrum',
        'electrum_gui',
        'electrum_gui.qt',
        'electrum_plugins',
        'electrum_plugins.audio_modem',
        'electrum_plugins.cosigner_pool',
        'electrum_plugins.email_requests',
        'electrum_plugins.greenaddress_instant',
        'electrum_plugins.hw_wallet',
        'electrum_plugins.keepkey',
        'electrum_plugins.labels',
        'electrum_plugins.ledger',
        'electrum_plugins.trezor',
        'electrum_plugins.digitalbitbox',
        'electrum_plugins.trustedcoin',
        'electrum_plugins.virtualkeyboard',
    ],
    package_dir={
        'electrum': 'lib',
        'electrum_gui': 'gui',
        'electrum_plugins': 'plugins',
    },
    package_data={
        'electrum': [
            'servers_bitcore.json',
            'servers_testnet.json',
            'currencies.json',
            'checkpoints.json',
            'checkpoints_testnet.json',
            'www/index.html',
            'wordlist/*.txt',
            'locale/*/LC_MESSAGES/electrum.mo',
        ]
    },
    scripts=['electrum-bitcore'],
    data_files=data_files,
    description="Lightweight BitCore Wallet",
    author="Thomas Voegtlin",
    author_email="thomasv@electrum.org",
    license="MIT Licence",
    url="https://bitcore.cc",
    long_description="""Lightweight BitCore Wallet""",
    ext_modules = [ext]
)
