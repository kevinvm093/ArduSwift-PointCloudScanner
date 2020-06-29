//
//  DataModel.swift
//  LidarScanner
//
//  Created by Kevin Vallejo on 6/17/20.
//  Copyright © 2020 Kevin Vallejo. All rights reserved.
//

import Foundation
import SceneKit
import QuartzCore

public class DataModel: NSObject {
    
    var xPoint:Float
    var yPoint:Float
    var zPoint:Float
    var sphereVector:SCNVector3
    
    init(withData:[Substring.SubSequence]) {
        
        self.xPoint = Float(withData[0])!
        self.yPoint = Float(withData[1])!
        self.zPoint = Float(withData[2])!
        self.sphereVector = SCNVector3(-Float(withData[3])!,
                                       -Float(withData[5])!,
                                       -Float(withData[4])!)
    }
}
