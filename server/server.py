import sys
import time
import logging
import socket
from ultralytics import YOLO
import numpy as np
import cv2
from PIL import Image
from asyncio.windows_events import NULL
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

class FileChangeHandler(FileSystemEventHandler):
    def __init__(self, model, client):
        self.model = model
        self.client = client

    def fighter_direction(self, boxes):
        coords = boxes.xywhn

        img_center_x = 0.5
        img_center_y = 0.5

        for coord in coords:
            x_center, y_center, _, _ = coord

            # 치우침 정도 계산
            horizontal_offset = x_center - img_center_x  # x 방향 차이
            vertical_offset = y_center - img_center_y    # y 방향 차이

        direction = ""
        # 방향 판단
        if abs(horizontal_offset) > 0.05:
            direction += 'L' if horizontal_offset < 0 else 'R'

        if abs(vertical_offset) > 0.05:
            direction += 'T' if vertical_offset < 0 else 'B'
        
        if direction == "":
            direction = 'C'

        return direction

    def on_modified(self, event):
        if event.src_path.endswith("C:\\Temp\\capture.png"):
            try:
                with open(event.src_path, "rb") as f:
                    frame = Image.open(f).convert("RGB")
                    frame.load()

                val = self.model(frame)
                for r in val:
                    boxes = r.boxes.cpu().numpy()
                    conf = boxes.conf
                    cls = boxes.cls

                if cls == 4:  # airplane class
                    str_msg = self.fighter_direction(boxes)
                    print("Detected airplane!")
                    print(f"Direction: {str_msg}")
                else:
                    str_msg = 'C'

                self.client.send(str_msg.encode('utf-8'))
            except Exception as e:
                print(f"Error processing image: {e}")

if __name__ == "__main__":
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('127.0.0.1', 6001))
    server.listen()

    model = YOLO('yolov5n.pt', verbose=False)
    logging.getLogger("ultralytics").setLevel(logging.ERROR)

    print("Server ready")
    client, address = server.accept()
    print(f"Connected to {address}")

    path_to_watch = "C:\\Temp\\"
    event_handler = FileChangeHandler(model, client)
    observer = Observer()
    observer.schedule(event_handler, path=path_to_watch, recursive=False)
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()
    client.close()
    server.close()
