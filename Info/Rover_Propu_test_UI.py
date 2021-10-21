#!/usr/bin/env python3

import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk
from gi.repository import GLib

import time
import serial

fwdStr = "forward;"
backwdStr = "backward;"
leftStr = "left_turn;"
rightStr = "right_turn;"
stopStr = "x;"

arduinoPort = "/dev/ttyACM0"
arduino = serial.Serial(port = arduinoPort, baudrate = 9600, timeout = 0.1)


class MainWindow(Gtk.Window):

    def __init__(self):
        super().__init__()

        self.init_ui()


    def init_ui(self):

        grid = Gtk.Grid()
        self.add(grid)

        self.fwdBtn = Gtk.Button(label="Forward")
        self.fwdBtn.set_size_request(80, 30)
        self.fwdBtn.connect("clicked", self.on_forward_clicked)
        
        self.backwdBtn = Gtk.Button(label="Backward")
        self.backwdBtn.set_size_request(80, 30)
        self.backwdBtn.connect("clicked", self.on_backward_clicked)

        self.timeEntry = Gtk.Entry()
        self.timeEntry.set_size_request(80,30)


        self.leftBtn = Gtk.Button(label="Left")
        self.leftBtn.set_size_request(80, 30)
        self.leftBtn.connect("clicked", self.on_left_clicked)
                
        self.rightBtn = Gtk.Button(label="Right")
        self.rightBtn.set_size_request(80, 30)
        self.rightBtn.connect("clicked", self.on_right_clicked)

        self.stopBtn= Gtk.Button(label="Stop")
        self.stopBtn.set_size_request(80, 30)
        self.stopBtn.connect("clicked", self.on_stop_clicked)

        self.serialLabel = Gtk.Label()
        GLib.timeout_add(200,self.update_serial)
        
        grid.attach(self.fwdBtn, 0, 0, 1, 1)
        grid.attach_next_to(self.backwdBtn, self.fwdBtn,Gtk.PositionType.RIGHT, 1, 1)
        grid.attach_next_to(self.timeEntry, self.backwdBtn,  Gtk.PositionType.RIGHT, 1,1)
        grid.attach_next_to(self.leftBtn, self.fwdBtn, Gtk.PositionType.BOTTOM, 1, 1)
        grid.attach_next_to(self.rightBtn, self.leftBtn, Gtk.PositionType.RIGHT, 1, 1)
        grid.attach_next_to(self.stopBtn, self.rightBtn, Gtk.PositionType.RIGHT, 1, 1)
        grid.attach_next_to(self.serialLabel, self.leftBtn, Gtk.PositionType.BOTTOM, 3, 1)
        
        

        self.set_border_width(10)
        self.set_title("Supaerover test")
        self.set_default_size(280, 180)
        self.connect("destroy", Gtk.main_quit)

    def on_forward_clicked(self, widget):
        arduino.write(fwdStr.encode())
        timeMs = self.timeEntry.get_text()+";"
        time.sleep(0.2)
        arduino.write(timeMs.encode())

    def on_backward_clicked(self,widget):
        arduino.write(backwdStr.encode())
        time.sleep(0.2)
        timeMs = self.timeEntry.get_text()+";"
        arduino.write(timeMs.encode())

    def on_left_clicked(self,widget):
        arduino.write(leftStr.encode())
        time.sleep(0.2)
        timems = self.timeEntry.get_text()+";"
        arduino.write(timems.encode())

    def on_right_clicked(self,widget):
        arduino.write(rightStr.encode())
        time.sleep(0.2)
        timems = self.timeEntry.get_text()+";"
        arduino.write(timems.encode())

    def on_stop_clicked(self,widget):
        arduino.write(stopStr.encode())

    def update_serial(self):
        while arduino.inWaiting():
            self.serialLabel.set_text('\n'.join(self.serialLabel.get_text().split(sep='\n')[-20:])+ arduino.readline().decode())
        return GLib.SOURCE_CONTINUE




win = MainWindow()
win.show_all()
Gtk.main()
