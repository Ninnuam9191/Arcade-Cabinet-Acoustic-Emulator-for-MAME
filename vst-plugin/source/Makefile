#!/usr/bin/make -f
# Makefile for ArcadeAmp
# Expects the DPF checkout (https://github.com/DISTRHO/DPF) next to this
# folder's parent, i.e.  <dir>/DPF  and  <dir>/ArcadeAmp  - see ../BUILD.md.

NAME = ArcadeAmp

FILES_UI = \
	ArcadeAmpUI.cpp

FILES_DSP = \
	ArcadeAmpPlugin.cpp

include ../DPF/Makefile.plugins.mk

TARGETS += vst2
TARGETS += vst3
TARGETS += clap

all: $(TARGETS)
