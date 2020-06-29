//
//  DataParser.swift
//  LidarScanner
//
//  Created by Kevin Vallejo on 6/17/20.
//  Copyright © 2020 Kevin Vallejo. All rights reserved.
//

import Foundation
import Cocoa

public class DataParser: NSObject{
    
    static let shared = DataParser()
    let file = File.shared
    @objc dynamic fileprivate(set) internal var dataPoints:[DataModel] = [DataModel]()
    
    // MARK: Public functions
    
    /// Reads coordinates stored in Text file and saves it into the model
    func readFile() {
        
        let now = DispatchTime.now()
        if let dir = FileManager.default.urls(for: .desktopDirectory, in: .allDomainsMask).first {
            
            let fileURL = dir.appendingPathComponent(file.name).appendingPathExtension(file.ext)
            do {
                
                let raw = try String(contentsOf: fileURL)
                let dataArray = raw.split(separator: file.newLineToken)
                
                for data in dataArray {
                    
                    dataPoints.append(DataModel(withData: data.split(separator: file.spaceToken)))
                }
            } catch { print("error reading file") }
        }
        
        let time = String((DispatchTime.now().uptimeNanoseconds - now.uptimeNanoseconds) / 1_000_000_000)
        print("Timer: \(time) seconds" )
    }
    
    /// reads stored coordinates from DataModel and copies them into a text file
    func writeFile() {
        let now = DispatchTime.now()
        
        if let dir = FileManager.default.urls(for: .documentDirectory, in: .allDomainsMask).first {
            var text = ""
            let fileURL = dir.appendingPathComponent("pcCoords").appendingPathExtension(file.ext)
            do {
                
                for data in dataPoints {
                    
                    text.append(contentsOf: "\(data.xPoint) \(data.yPoint) \(data.zPoint) \(data.sphereVector.x) \(data.sphereVector.z) \(data.sphereVector.y)\n")
                }
                
                try text.write(to: fileURL, atomically: true, encoding: String.Encoding.utf8)
            } catch { print("error writing to file") }
            
              let time = String((DispatchTime.now().uptimeNanoseconds - now.uptimeNanoseconds) / 1_000_000_000)
            print("Timer: \(time) seconds" )
        }
        
    }
    
    /// Parses serial data coming in from arduino.
    func parseData(data:Data) {
        
        let dataString = String(decoding: data, as: UTF8.self)
        dataPoints.append(DataModel(withData: dataString.split(separator: file.spaceToken)))
    }
}
