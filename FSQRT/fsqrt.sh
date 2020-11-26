#!/bin/sh

xvlog -sv test_fsqrt.sv fsqrt.v
xelab -debug typical test_fsqrt -s test_fsqrt.sim
xsim --runall test_fsqrt.sim

