#!/usr/bin/env python3
#coding: utf-8

import licant

licant.include("linalg-v3")
licant.include("malgo")
licant.include("nos")
licant.include("gxx")

licant.cxx_application("target",
	sources=["main.cpp"],
	mdepends=["linalg-v3", "nos", "gxx", "malgo", 
		("gxx.dprint", "__none__"),
	]
)

licant.ex("target")