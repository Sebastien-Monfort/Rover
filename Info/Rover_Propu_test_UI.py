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

arduinoPort = "/dev/ttyACM0"
#arduino = serial.Serial(port = arduinoPort, baudrate = 9600, timeout = 0.1)


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
        #self.timeEntry.set_size_request(80,30)

        self.leftBtn = Gtk.Button(label="Left")
        self.leftBtn.set_size_request(80, 30)
        self.leftBtn.connect("clicked", self.on_left_clicked)
                
        self.rightBtn = Gtk.Button(label="Right")
        self.rightBtn.set_size_request(80, 30)
        self.rightBtn.connect("clicked", self.on_right_clicked)

        self.serialLabel = Gtk.Label()
        GLib.timeout_add(200,self.update_serial)
        
        grid.attach(self.fwdBtn, 0, 0, 1, 1)
        grid.attach_next_to(self.backwdBtn, self.fwdBtn,Gtk.PositionType.RIGHT, 1, 1)
        grid.attach_next_to(self.timeEntry, self.backwdBtn,  Gtk.PositionType.RIGHT, 1,1)
        grid.attach_next_to(self.leftBtn, self.fwdBtn, Gtk.PositionType.BOTTOM, 1, 1)
        grid.attach_next_to(self.rightBtn, self.leftBtn, Gtk.PositionType.RIGHT, 1, 1)
        

        self.set_border_width(10)
        self.set_title("Supaerover test")
        self.set_default_size(280, 180)
        self.connect("destroy", Gtk.main_quit)

    def on_forward_clicked(self, widget):
        print(fwdStr)
        #arduino.write(fwdStr)
        timeMs = self.timeEntry.get_text()+";"
        time.sleep(1)
        print(timeMs)
        #arduino.write(timeMs)

    def on_backward_clicked(self,widget):
        print(backwdStr)
        #arduino.write(backwdStr)
        time.sleep(1)
        timeMs = self.timeEntry.get_text()+";"
        print(timeMs)
        #arduino.write(timeMs)

    def on_left_clicked(self,widget):
        print(leftStr)
        #arduino.write(leftStr)
        time.sleep(1)
        timems = self.timeEntry.get_text()+";"
        print(timems)
        #arduino.write(timems)

    def on_right_clicked(self,widget):
        print(rightStr)
        #arduino.write(rightStr)
        time.sleep(1)
        timems = self.timeEntry.get_text()+";"
        print(timems)
        #arduino.write(timems)

    def update_serial(self):
        print("blop")
        return GLib.SOURCE_CONTINUE




win = MainWindow()
win.show_all()
Gtk.main()
