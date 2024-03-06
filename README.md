# Knee Joint Motion Degree Monitoring System

## Project Overview
### Description
The Knee Joint Motion Monitoring System is designed to analyze and monitor the motion of a replaced knee joint rehabilitation. It utilizes two Inertial Measurement Units (IMUs) placed at each end of the joint for angle measurement and motion recognition. The system includes a sensing device and a display device, each with its custom PCB.

### Sensing Device 
The sensor design incorporates two flexible straps, strapped around the thigh and calf, to monitor knee joint movements. Power and data transmission are facilitated through robust wiring for maximum reliability. The TCA9548A multiplexer is utilized for efficient management and organization of sensor data.

![Slide Sensor](https://github.com/ShunxiWu/TECHIN514-Project/blob/main/pictures/514-slide1.png)

### Display Device 
The display device utilizes a precise NEMA 17 bipolar stepper motor and WS2812B RGB LEDs to visually indicate knee range of motion. It includes a strategically placed capacitive touch button for user interaction. The system is powered by a 3000mAh Lithium Polymer battery, ensuring optimal performance for approximately 48 hours. Custom DSP/ML algorithms on the sensing device and a custom PCB enhance efficiency and integration.


![Slide Display](https://github.com/ShunxiWu/TECHIN514-Project/blob/main/pictures/514-slide2.png)

### Mechanism
The sensing device and display device communicate wirelessly using Bluetooth for efficient data transmission. The sensing device, with IMUs and a TCA9548A multiplexer, captures knee motion data, processes it using custom algorithms, and wirelessly transmits the organized information to the display device. The display device, equipped with a stepper motor, LEDs, and capacitive touch button, receives the data and visually represents knee joint motion based on the transmitted information.

![Slide Sensor](https://github.com/ShunxiWu/TECHIN514-Project/blob/main/pictures/514-slide3.png)


## Sensing Device

### Basic Enclosure
**Enclosure Design:** [Describe the basic enclosure design for the sensing device]

<!-- ### Custom PCB
**PCB Design:** [Describe the custom PCB design for the sensing device] -->

### IMU 1 and 2:
**Part Number:** JY901

**Description:**
IMU 1 and 2 is placed at each one end of the knee joint to measure angles and capture motion data. It provides accurate information for joint movement analysis.

<!-- **Datasheets:** -->


### TCA9548A - I2C Multiplexer
**Part Number:** TCA9548A

**Description:**
The TCA9548A is an I2C multiplexer with eight bidirectional translating switches that can be controlled through the I2C bus. It provides flexibility for expanding one upstream pair (SCL/SDA) to eight downstream pairs or channels.
The TCA9548A is utilized in our Knee Joint Motion Monitoring System to enable the selection of different downstream channels for I2C communication with multiple IMUs. This enhances the system's flexibility and resolves potential address conflicts.

### ESP32 - Wireless Communication
**Wireless Module:** ESP32 Internal Wi-Fi/Bluetooth Module

**Description:**
The sensing device communicates wirelessly with the display device using the ESP32's internal Wi-Fi and/or Bluetooth capabilities. The ESP32 microcontroller is equipped with integrated Wi-Fi (802.11 b/g/n) and Bluetooth (Classic and BLE) modules, providing reliable and versatile wireless communication options.

**Integration in the Project:**
- Wi-Fi: Enables data transfer and communication over local Wi-Fi networks, facilitating remote monitoring and data logging.
- Bluetooth: Facilitates short-range communication between the sensing and display devices, ensuring seamless connectivity.

<!-- **Datasheets:** -->

### Power Solutions
**Battery Type:** Lithium Polymer (Li-Po)

**Description:**
The Knee Joint Motion Monitoring System is powered by a lightweight and high-energy-density Lithium Polymer (Li-Po) battery. Li-Po batteries are known for their compact size and weight, making them suitable for portable and wearable applications. The specific battery model and manufacturer details can be found in the "Datasheets" folder.

**Capacity:** 3000mAh


**Description:**
Power is provided by a [insert battery type] with [insert capacity]. A detailed analysis of battery-only options is provided in the project documentation.

<!-- **Datasheets:** -->



## Display Device
### Custom Enclosure
**Enclosure Design:** [Describe the custom enclosure design for the display device]

### Custom PCB
**PCB Design:** [Describe the custom PCB design for the display device]

### Stepper-Motor-Driven Gauge Needle and LED
**Stepper Motor:** NEMA 17 Bipolar Stepper Motor (e.g., Model: 17HS4401)

**LEDs:** WS2812B RGB LEDs

**Description:**
The display device features a precision NEMA 17 bipolar stepper motor, such as the Model 17HS4401, driving a gauge needle to visually represent the knee's range of motion. This stepper motor provides accurate and controlled motion, allowing users to monitor and interpret the knee joint's movement. Additionally, WS2812B RGB LEDs are integrated to offer dynamic and customizable lighting for enhanced visual feedback.

### Button
**Button Type:** Capacitive Touch Button

**Description:**
Capacitive touch buttons provide a seamless and responsive interface for users to turn on and off the display device.

### Thoughtfully-Sized Battery
**Battery Type:** Lithium Polymer (Li-Po)

**Capacity:** 3000mAh

**Estimated Runtime:** Approximately 48 hours

**Description:**
The display device is powered by a lightweight and high-energy-density Lithium Polymer (Li-Po) battery with a capacity of 3000mAh. This thoughtfully-sized battery ensures optimal performance and provides an estimated runtime of approximately 48 hours on a full charge. The battery type and capacity are chosen to balance extended runtime with the need for a lightweight solution.

<!-- **Datasheets:** -->


<!-- ### Additional Displays (Optional)
[Include details if additional displays (OLED, LCD, E-INK, etc.) are incorporated.] -->

## Additional Requirements
## Additional Requirements

### DSP/ML Algorithms
**Algorithms Developed:** Motion Recognition Algorithm for Knee Joint Monitoring

**Description:**
Custom DSP/ML algorithms have been developed to handle motion data from the Inertial Measurement Units (IMUs). The Motion Recognition Algorithm enables accurate analysis of knee joint movements. These algorithms are implemented on the microcontroller of the sensing device, providing real-time processing and valuable insights into the user's knee motion.

### Custom PCB Incorporation
**Location of Custom PCB:** Sensing Device

**Description:**
At least one custom PCB is incorporated into the final project, enhancing the functionality and efficiency of the sensing device. The custom PCB facilitates seamless integration of components, including IMUs, TCA9548A multiplexer, and the microcontroller (e.g., ESP32). It optimizes the layout for minimal power consumption and ensures precise data acquisition for the DSP/ML algorithms. The custom PCB design reflects a thoughtful approach to compact and efficient hardware integration.


