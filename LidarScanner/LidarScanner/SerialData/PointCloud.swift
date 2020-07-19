//
//  DataParser.swift
//  LidarScanner
//
//  Created by Kevin Vallejo on 6/17/20.
//  Copyright © 2020 Kevin Vallejo. All rights reserved.
//

import Foundation
import Cocoa
import SceneKit

public class PointCloud: NSObject {
    
    static let shared = PointCloud()
    var vectors = [simd_float3]()
    @objc dynamic fileprivate(set) internal var flag = false
    let file = File.shared

    
    // MARK: - Public functions
    
    /// Reads coordinates stored in Text file and saves it into the model
    func readFile() {
        
        let now = DispatchTime.now()
        if let dir = FileManager.default.urls(for: .desktopDirectory, in: .allDomainsMask).first {
            
            let fileURL = dir.appendingPathComponent(file.name).appendingPathExtension(file.ext)
            do {
                
                let raw = try String(contentsOf: fileURL)
                let dataArray = raw.split(separator: file.newLineToken)
                for data in dataArray {
                    
                    let d = data.split(separator: file.spaceToken)
                    vectors.append(simd_float3(Float(d[0])!, Float(d[1])!, Float(d[2])!))
                    flag = false
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
            let fileURL = dir.appendingPathComponent(file.name).appendingPathExtension(file.ext)
            do {
                
                for data in vectors {
                        text.append(contentsOf: "\(data.x) \(data.z) \(data.y)\n")
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
        let d = dataString.split(separator: file.spaceToken)
        vectors.append(simd_float3(Float(d[0])!, Float(d[1])!, Float(d[2])!))
    }
    
    var colorHSB:NSColor {

        let h = ((vectors.last?.getAngle())!) / .pi + 0.25
        return NSColor.init(calibratedHue: h, saturation: 1.0, brightness: 1.0, alpha: 1.0)
    }
}
