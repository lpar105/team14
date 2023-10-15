# THIS FILE IS AUTOMATICALLY GENERATED
# Project: H:\Documents\GitHub\team14\301_lab_tasks\psoc student pack\psoc_code_base\CS301_Class.cydsn\CS301_Class.cyprj
# Date: Sat, 14 Oct 2023 22:36:33 GMT
#set_units -time ns
create_clock -name {ADC1_IntClock(routed)} -period 999.99999999999989 -waveform {0 500} [list [get_pins {ClockBlock/dclk_3}]]
create_clock -name {CyILO} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 16.666666666666664 -waveform {0 8.33333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 16.666666666666664 -waveform {0 8.33333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {Clock_QENC} -source [get_pins {ClockBlock/clk_sync}] -edges {1 5 11} -nominal_period 83.333333333333314 [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {Clock_PWM} -source [get_pins {ClockBlock/clk_sync}] -edges {1 61 121} [list [get_pins {ClockBlock/dclk_glb_2}]]
create_generated_clock -name {ADC1_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 61 121} [list [get_pins {ClockBlock/dclk_glb_3}]]
create_generated_clock -name {UART_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 131 261} [list [get_pins {ClockBlock/dclk_glb_4}]]
create_generated_clock -name {Clock2} -source [get_pins {ClockBlock/clk_sync}] -edges {1 61 121} [list [get_pins {ClockBlock/dclk_glb_1}]]

set_false_path -from [get_pins {__ONE__/q}]

# Component constraints for H:\Documents\GitHub\team14\301_lab_tasks\psoc student pack\psoc_code_base\CS301_Class.cydsn\TopDesign\TopDesign.cysch
# Project: H:\Documents\GitHub\team14\301_lab_tasks\psoc student pack\psoc_code_base\CS301_Class.cydsn\CS301_Class.cyprj
# Date: Sat, 14 Oct 2023 22:34:36 GMT
