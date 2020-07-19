//
//  UARThandler.swift
//  LidarScanner
//
//  Created by Kevin Vallejo on 6/25/20.
//  Copyright © 2020 Kevin Vallejo. All rights reserved.
//

import Foundation
import ORSSerial

class UARThandler : NSObject, ORSSerialPortDelegate {
    
    let standardInputFileHandle = FileHandle.standardInput
    let pointCloud = PointCloud.shared
    var serialPort: ORSSerialPort?

    override init() {
        super.init()

        let port = ORSSerialPortManager.shared().availablePorts.first
        print(port!.path)
        
        if let port = ORSSerialPort(path: port!.path) {
            port.baudRate = 115200
            port.delegate = self
            port.rts = true
            port.open()
            self.serialPort = port
        }
        else { print("Serial port error") }
    }

    // MARK: - ORSSerialPortDelegate
    
    func serialPortWasOpened(_ serialPort: ORSSerialPort) {
        
        print("Serial port \(serialPort) was opened!")
        let descriptor = ORSSerialPacketDescriptor(prefixString: "<", suffixString: ">",
                                                   maximumPacketLength: 34, userInfo: nil)
        self.serialPort?.startListeningForPackets(matching: descriptor)
    }
    
    func serialPort(_ serialPort: ORSSerialPort, didReceivePacket packetData: Data,
                    matching descriptor: ORSSerialPacketDescriptor) {
        
        pointCloud.parseData(data: packetData.dropFirst().dropLast())
    }
 
    func serialPortWasRemovedFromSystem(_ serialPort: ORSSerialPort) {
        
        self.serialPort = nil
    }
    
    func serialPortWasRemoved(fromSystem fromFileHandlelPort: ORSSerialPort) {
        
        self.serialPort = nil
    }

    private func serialPort(serialPort: ORSSerialPort, didEncounterError error: NSError) {
        
        print("Serial port (\(serialPort)) encountered error: \(error)")
    }
}
