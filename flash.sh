#!/bin/bash

nrfjprog --family NRF52 --program outdir/nrf52_vbluno52/zephyr.hex --chiperase --verify; nrfjprog --family NRF52 --reset
