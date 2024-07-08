# Predictive Industrial Maintenance Visualization 

## Overview
This project is part of the ICT for Industrial Automation course, focusing on predictive industrial maintenance. The goal is to visualize sensor data over time to predict and maintain the industrial machinery efficiently. The provided data includes various sensor readings such as vibration levels, sound levels, HP, life span, and more. This script generates an interactive plot that displays these sensor readings over time, allowing for better analysis and decision-making.

## Requirements
Arduino IDE
Python 3.7 or higher
Pandas
Plotly
Openpyxl

## Data
The data is provided in an Excel file data.xlsx with the following columns:

 -Vibration Level
 -Vibration Intensity
 -Sound Level
 -Sound Value
 -HP
 -Life Span % remaining
 -Damage
 -Time
 
Each row represents sensor readings at a specific time step.


## Running the Script

Save the provided script
Execute the script from your terminal: python project_ICT.py


## View the Output:

The script will generate an interactive_plot.html file in your working directory.
Open interactive_plot.html in your web browser to view the interactive plot.

## Conclusion
This script provides a comprehensive visualization of sensor data over time, which is essential for predictive industrial maintenance. By analyzing these plots, one can identify patterns and anomalies, leading to more informed maintenance decisions and minimizing machinery downtime.
