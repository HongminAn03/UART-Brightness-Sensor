#!/usr/bin/env python3
import traceback

from openpyxl import Workbook
from datetime import datetime
import serial
import serial.tools.list_ports
import time
import cmd
import sys
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque


def find_usb_serial(description_keyword="FT232R USB UART"):
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if description_keyword in port.description:
            return port.device
    return None


class SerialShell(cmd.Cmd):
    intro = "Welcome to the Serial Shell. Type h to list commands.\n"
    prompt = "serial> "


    def __init__(self, port, baudrate=115200):
        super().__init__()
        self.ser = serial.Serial(port, baudrate, timeout=1)
        print(f"Connected to {port}")

    def do_e(self, arg):
        """Exit the shell"""
        print("Closing serial connection...")
        self.save_excel_file()  # Save before exiting
        self.ser.close()
        return True

    def setup_excel_logger(self):
        """Initialize Excel workbook"""
        self.wb = Workbook()
        self.ws = self.wb.active
        self.ws.title = "Brightness Data"
        self.ws.append(["Timestamp", "Time (ms)", "Brightness"])
        self.excel_filename = f"brightness_log_{datetime.now().strftime('%Y%m%d_%H%M%S')}.xlsx"

    def log_to_excel(self, time_ms, brightness):
        """Record data to Excel"""
        self.ws.append([
            datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
            time_ms,
            brightness
        ])

    def save_excel_file(self):
        """Save and close the Excel file"""
        try:
            self.wb.save(self.excel_filename)
            print(f"Data saved to {self.excel_filename}")
        except Exception as e:
            print(f"Error saving Excel file: {e}")

    def do_d(self, arg):
        """Write data sheet to serial device"""
        self.ser.write("q".encode())
        time.sleep(0.01)
        self.ser.write("q".encode())
        time.sleep(0.01)
        self.ser.write("q".encode())
        time.sleep(0.01)
        self.ser.write("q".encode())
        time.sleep(0.01)
        self.ser.write("i".encode())
        time.sleep(0.01)
        self.ser.write("w 33 7c\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 00 1E\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 01 0a\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 02 0f\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 03 05\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 04 00\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 05 10\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 06 10\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 07 40\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 08 C0\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 09 00\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0A 00\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0B 00\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0C F4\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0D F4\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0E F4\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0F F4\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 10 84\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 11 7F\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 12 06\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 13 10\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 14 20\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 15 70\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 17 01\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 60 14\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 18 00\r\n".encode())
        self.ser.write("w 19 43\r\n".encode())
        self.ser.write("w 1A 06\r\n".encode())
        self.ser.write("w 1B 00\r\n".encode())
        self.ser.write("w 32 EF\r\n".encode())
        self.ser.write("w 38 00\r\n".encode())
        self.ser.write("w 39 00\r\n".encode())
        self.ser.write("w 40 64\r\n".encode())
        self.ser.write("w 41 00\r\n".encode())
        self.ser.write("w 42 29\r\n".encode())
        self.ser.write("w 45 29\r\n".encode())
        self.ser.write("w 3A 00\r\n".encode())
        self.ser.write("w 34 04\r\n".encode())
        self.ser.write("w 3C 00\r\n".encode())
        self.ser.write("w 3E ff\r\n".encode())
        self.ser.write("w 3F 00\r\n".encode())
        self.ser.write("w 50 00\r\n".encode())
        self.ser.write("w 51 00\r\n".encode())
        self.ser.write("w 52 00\r\n".encode())
        self.ser.write("w 53 00\r\n".encode())
        self.ser.write("w 54 00\r\n".encode())
        self.ser.write("w 55 00\r\n".encode())
        self.ser.write("w 56 00\r\n".encode())
        self.ser.write("w 57 00\r\n".encode())
        self.ser.write("w 58 06\r\n".encode())
        self.ser.write("w 48 00\r\n".encode())
        self.ser.write("w 49 00\r\n".encode())
        self.ser.write("w 35 20\r\n".encode())
        self.ser.write("w 36 20\r\n".encode())
        self.ser.write("w 37 20\r\n".encode())
        time.sleep(0.1)

        """Read lines sent from serial to check data"""

        while self.ser.in_waiting > 0:
            response = self.ser.readline().decode(errors='ignore').strip()
            print(response)
            time.sleep(0.01)



    def do_g(self, arg):
        """Write Data Sheet"""
        self.ser.write("q".encode())
        time.sleep(0.01)
        self.ser.write("q".encode())
        time.sleep(0.01)
        self.ser.write("q".encode())
        time.sleep(0.01)
        self.ser.write("q".encode())
        time.sleep(0.01)
        self.ser.write("i".encode())
        time.sleep(0.01)
        self.ser.write("w 33 7c\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 00 1E\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 01 0a\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 02 0f\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 03 05\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 04 00\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 05 10\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 06 10\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 07 40\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 08 C0\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 09 00\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0A 00\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0B 00\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0C F4\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0D F4\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0E F4\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 0F F4\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 10 84\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 11 7F\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 12 06\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 13 10\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 14 20\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 15 70\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 17 01\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 60 14\r\n".encode())
        time.sleep(0.01)
        self.ser.write("w 18 00\r\n".encode())
        self.ser.write("w 19 43\r\n".encode())
        self.ser.write("w 1A 06\r\n".encode())
        self.ser.write("w 1B 00\r\n".encode())
        self.ser.write("w 32 EF\r\n".encode())
        self.ser.write("w 38 00\r\n".encode())
        self.ser.write("w 39 00\r\n".encode())
        self.ser.write("w 40 64\r\n".encode())
        self.ser.write("w 41 00\r\n".encode())
        self.ser.write("w 42 29\r\n".encode())
        self.ser.write("w 45 29\r\n".encode())
        self.ser.write("w 3A 00\r\n".encode())
        self.ser.write("w 34 04\r\n".encode())
        self.ser.write("w 3C 00\r\n".encode())
        self.ser.write("w 3E ff\r\n".encode())
        self.ser.write("w 3F 00\r\n".encode())
        self.ser.write("w 50 00\r\n".encode())
        self.ser.write("w 51 00\r\n".encode())
        self.ser.write("w 52 00\r\n".encode())
        self.ser.write("w 53 00\r\n".encode())
        self.ser.write("w 54 00\r\n".encode())
        self.ser.write("w 55 00\r\n".encode())
        self.ser.write("w 56 00\r\n".encode())
        self.ser.write("w 57 00\r\n".encode())
        self.ser.write("w 58 06\r\n".encode())
        self.ser.write("w 48 00\r\n".encode())
        self.ser.write("w 49 00\r\n".encode())
        self.ser.write("w 35 20\r\n".encode())
        self.ser.write("w 36 20\r\n".encode())
        self.ser.write("w 37 20\r\n".encode())
        time.sleep(0.1)

        """Generate a graph with time vs. brightness"""
        # Setup the figure and axis
        self.setup_excel_logger()

        fig, ax = plt.subplots(figsize=(10, 6))
        ax.set_xlim(0, 1000)
        ax.set_ylim(0, 15000)
        plt.title("Real-time Brightness Monitoring")
        plt.xlabel("Time")
        plt.ylabel("Brightness Level")
        plt.grid(True)

        # Initialize line
        bw_line, = ax.plot([], [], 'b-', label='Brightness Value')
        plt.legend()

        # Initialize data storage
        x_data = deque(maxlen=1000)
        bw_data = deque(maxlen=1000)

        def get_sensor_values():
            """Helper function to read sensor data"""
            self.ser.write("r 1d\r\n".encode())
            self.ser.write("r 1e\r\n".encode())
            self.ser.write("r 1f\r\n".encode())
            self.ser.write("r 20\r\n".encode())

            values = []
            while len(values) < 4 and self.ser.in_waiting:
                line = self.ser.readline()
                try:
                    decoded = line.decode(errors='ignore').strip()
                    parts = decoded.split()
                    if parts and parts[-1].startswith("0x"):
                        values.append(decoded)
                except UnicodeDecodeError:
                    continue

            r, g, b, w = (values + [None] * 4)[:4]

            def safe_hex_extract(s):
                if s is None:
                    return 0
                try:
                    return int(s.split()[-1][2:], 16)
                except Exception:
                    return 0

            b_val = safe_hex_extract(b)
            w_val = safe_hex_extract(w)
            return (b_val << 8) | w_val

        def animate(i):
            """Animation update function"""
            try:
                bw_value = get_sensor_values()
                x_data.append(i)
                bw_data.append(bw_value)

                # Log to Excel
                self.log_to_excel(i, bw_value)  # New function call

                bw_line.set_data(list(x_data), list(bw_data))

                bw_line.set_data(list(x_data), list(bw_data))

                # Auto-scaling
                if i > ax.get_xlim()[1] * 0.9:
                    ax.set_xlim(i - 50, i + 10)

                current_max = max(bw_data[-50:]) if bw_data else 15000
                ax.set_ylim(0, current_max * 1.1)

                return (bw_line,)  # Must return a tuple of Artists

            except Exception as e:
                return (bw_line,)  # Always return a tuple

        # Create animation
        ani = animation.FuncAnimation(
            fig,
            animate,
            frames=None,
            interval=100,
            blit=True,
            cache_frame_data=False
        )

        plt.tight_layout()
        plt.show()



    def do_h(self, arg):
            """List available commands and general usage"""
            print("Type any command to send to the serial device.")
            print("Special commands:")
            print("    h     Print this help screen")
            print("    e     Exit the shell")
            print("    d     Write data sheet")
            print("    g     Generate real-time graph")

def postloop(self):
    print("Session ended.")



if __name__ == "__main__":
    port = find_usb_serial()
    if not port:
        print("No USB serial device found.")
        sys.exit(1)

    shell = SerialShell(port)
    try:
        shell.cmdloop()
    except KeyboardInterrupt:
        print("\nInterrupted by user.")
        shell.ser.close()
