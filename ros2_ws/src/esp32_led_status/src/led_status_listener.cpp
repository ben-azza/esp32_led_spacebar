#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"

class LedStatusListener : public rclcpp::Node
{
public:
  LedStatusListener() : Node("led_status_listener")
  {
    subscription_ = this->create_subscription<std_msgs::msg::Bool>(
      "/led_cmd", 10,
      std::bind(&LedStatusListener::topic_callback, this, std::placeholders::_1));
  }

private:
  void topic_callback(const std_msgs::msg::Bool & msg) const
  {
    RCLCPP_INFO(this->get_logger(), "LED -> %s", msg.data ? "ON" : "OFF");
  }
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LedStatusListener>());
  rclcpp::shutdown();
  return 0;
}
