#!/usr/bin/env python3

import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.include("malgo")

tests = [
	"vector",
]

application("runtests",
	sources = ["main.cpp"] + [t+".cpp" for t in tests],
	mdepends = [
		"malgo"
	],

	libs = ["gtest", "pthread"]
)

licant.ex("runtests")