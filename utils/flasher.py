import sys
import os
import subprocess
import json
from PyQt6.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QLabel, QPushButton, 
    QFileDialog, QComboBox, QMessageBox, QProgressBar, QHBoxLayout
)
from PyQt6.QtCore import QThread, pyqtSignal

def list_removable_drives():
    try:
        result = subprocess.run(['lsblk', '-J', '-o', 'NAME,RM,SIZE,MODEL,MOUNTPOINT'], capture_output=True, text=True, check=True)
        drives = json.loads(result.stdout)['blockdevices']
        removable = [d for d in drives if d['rm'] == True]
        return removable
    except Exception as e:
        return []

class FlashThread(QThread):
    progress = pyqtSignal(int)
    finished = pyqtSignal(bool, str)

    def __init__(self, device, image):
        super().__init__()
        self.device = device
        self.image = image

    def run(self):
        try:
            # Run dd with status=progress, parse progress output
            dd_cmd = ['dd', f'if={self.image}', f'of={self.device}', 'bs=4M', 'status=progress', 'oflag=sync']
            process = subprocess.Popen(dd_cmd, stderr=subprocess.PIPE, text=True)

            for line in process.stderr:
                # Example dd progress line: 12345678 bytes (12 MB, 11 MiB) copied, 2 s, 6.3 MB/s
                if "bytes" in line:
                    try:
                        parts = line.split()
                        bytes_copied = int(parts[0])
                        # Rough estimate progress:
                        image_size = os.path.getsize(self.image)
                        progress_percent = int(bytes_copied / image_size * 100)
                        if progress_percent > 100:
                            progress_percent = 100
                        self.progress.emit(progress_percent)
                    except:
                        pass

            retcode = process.wait()
            if retcode == 0:
                self.finished.emit(True, "Flashing completed successfully.")
            else:
                self.finished.emit(False, "dd exited with error.")
        except Exception as e:
            self.finished.emit(False, f"Error: {str(e)}")


class USBFlasher(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("ArcOS USB Flasher")
        self.setMinimumSize(200, 500)
        self.device = None
        self.image = None

        layout = QVBoxLayout()

        self.device_label = QLabel("Select USB Device:")
        self.device_combo = QComboBox()
        layout.addWidget(self.device_label)
        layout.addWidget(self.device_combo)

        self.image_label = QLabel("No image selected")
        self.select_image_btn = QPushButton("Select Image File")
        self.select_image_btn.clicked.connect(self.select_image)
        layout.addWidget(self.image_label)
        layout.addWidget(self.select_image_btn)

        self.flash_btn = QPushButton("Flash")
        self.flash_btn.clicked.connect(self.flash_image)
        self.flash_btn.setEnabled(False)
        layout.addWidget(self.flash_btn)

        self.progress_bar = QProgressBar()
        self.progress_bar.setValue(0)
        layout.addWidget(self.progress_bar)

        self.setLayout(layout)

        self.refresh_devices()

    def refresh_devices(self):
        self.device_combo.clear()
        drives = list_removable_drives()
        if not drives:
            self.device_combo.addItem("No removable drives found")
            self.device_combo.setEnabled(False)
            self.flash_btn.setEnabled(False)
        else:
            for d in drives:
                desc = f"/dev/{d['name']} - {d['model']} - {d['size']}"
                self.device_combo.addItem(desc, f"/dev/{d['name']}")
            self.device_combo.setEnabled(True)
            self.flash_btn.setEnabled(bool(self.image))

    def select_image(self):
        file_dialog = QFileDialog()
        file_path, _ = file_dialog.getOpenFileName(self, "Select ISO/Image File", "", "Image Files (*.iso *.img);;All Files (*)")
        if file_path:
            self.image = file_path
            self.image_label.setText(f"Selected: {os.path.basename(file_path)}")
            self.flash_btn.setEnabled(True if self.device_combo.isEnabled() else False)

    def flash_image(self):
        self.device = self.device_combo.currentData()
        if not self.device or not self.image:
            QMessageBox.warning(self, "Error", "Please select a device and an image file.")
            return

        reply = QMessageBox.question(
            self, "Confirm Flash",
            f"Are you sure you want to flash\n{os.path.basename(self.image)}\nto\n{self.device}?\nThis will erase all data on the device!",
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No
        )

        if reply != QMessageBox.StandardButton.Yes:
            return

        self.flash_btn.setEnabled(False)
        self.select_image_btn.setEnabled(False)
        self.device_combo.setEnabled(False)
        self.progress_bar.setValue(0)

        self.thread = FlashThread(self.device, self.image)
        self.thread.progress.connect(self.progress_bar.setValue)
        self.thread.finished.connect(self.flash_finished)
        self.thread.start()

    def flash_finished(self, success, message):
        QMessageBox.information(self, "Flashing Result", message)
        self.flash_btn.setEnabled(True)
        self.select_image_btn.setEnabled(True)
        self.device_combo.setEnabled(True)
        self.progress_bar.setValue(0)


if __name__ == "__main__":
    if os.geteuid() != 0:
        print("Please run this script as root or with sudo.")
        sys.exit(1)

    app = QApplication(sys.argv)
    flasher = USBFlasher()
    flasher.show()
    sys.exit(app.exec())

