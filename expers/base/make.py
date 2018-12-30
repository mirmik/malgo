#!/usr/bin/env python3
#coding: utf-8

import licant

licant.include("linalg-v3")
licant.include("malgo")
licant.include("nos")

licant.cxx_application("target",
	sources=["main.cpp"],
	mdepends=["linalg-v3", "nos", "malgo"]
)

licant.ex("target")