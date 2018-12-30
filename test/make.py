#!/usr/bin/env python3

import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.execute("../nos.g.py")
licant.include("gxx")

tests = [
	"print",
	"fprint",
	#"log",
]

application("runtests",
	sources = ["main.cpp"] + [t+".cpp" for t in tests],

	ld_flags = "-L/usr/local/lib/",

	include_paths = ["."],
	mdepends = [
		"nos",
		("gxx.dprint", "stdout")
	],

	libs = ["gtest", "pthread"]
)

licant.ex("runtests")