import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/mouha/esp32_led_spacebar/ros2_ws/install/esp32_led_teleop'
