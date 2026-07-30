#!/usr/bin/env python3
import sys
import termios
import tty
import threading

import rclpy
from rclpy.node import Node
from std_msgs.msg import Bool


class KeyboardLedPublisher(Node):
    def __init__(self):
        super().__init__('keyboard_led_publisher')
        self.publisher_ = self.create_publisher(Bool, '/led_cmd', 10)
        self.led_state = False
        self.get_logger().info(
            "Pret. Appuie sur ESPACE pour basculer la LED. 'q' pour quitter."
        )

    def toggle_led(self):
        self.led_state = not self.led_state
        msg = Bool()
        msg.data = self.led_state
        self.publisher_.publish(msg)
        self.get_logger().info(f"LED -> {'ON' if self.led_state else 'OFF'}")


def get_key(settings):
    tty.setraw(sys.stdin.fileno())
    key = sys.stdin.read(1)
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key


def main(args=None):
    rclpy.init(args=args)
    node = KeyboardLedPublisher()

    spin_thread = threading.Thread(target=rclpy.spin, args=(node,), daemon=True)
    spin_thread.start()

    settings = termios.tcgetattr(sys.stdin)
    try:
        while rclpy.ok():
            key = get_key(settings)
            if key == ' ':
                node.toggle_led()
            elif key == 'q':
                break
    except KeyboardInterrupt:
        pass
    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
