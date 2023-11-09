# LEDS
set_property  -dict {PACKAGE_PIN A12 IOSTANDARD LVCMOS18} [get_ports {LEDS[0]}]
set_property  -dict {PACKAGE_PIN A11 IOSTANDARD LVCMOS18} [get_ports {LEDS[1]}]
set_property  -dict {PACKAGE_PIN A9  IOSTANDARD LVCMOS18} [get_ports {LEDS[2]}]
set_property  -dict {PACKAGE_PIN A10 IOSTANDARD LVCMOS18} [get_ports {LEDS[3]}]

# SWITCHES
set_property  -dict {PACKAGE_PIN A2  IOSTANDARD LVCMOS25} [get_ports {SWITCHES[0]}]
set_property  -dict {PACKAGE_PIN A5  IOSTANDARD LVCMOS25} [get_ports {SWITCHES[1]}]
set_property  -dict {PACKAGE_PIN B5  IOSTANDARD LVCMOS25} [get_ports {SWITCHES[2]}]
set_property  -dict {PACKAGE_PIN A3  IOSTANDARD LVCMOS25} [get_ports {SWITCHES[3]}]
