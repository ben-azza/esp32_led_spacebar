from setuptools import find_packages, setup

package_name = 'esp32_led_teleop'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='mouha',
    maintainer_email='mohamedyassine.benazza@insat.ucar.tn',
    description='TODO: Package description',
    license='MIT',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'keyboard_led_publisher = esp32_led_teleop.keyboard_led_publisher:main'
        ],
    },
)
