//
//  PointCloud.swift
//  LidarScanner
//
//  Created by Kevin Vallejo on 7/4/20.
//  Copyright © 2020 Kevin Vallejo. All rights reserved.
//

import SceneKit
import QuartzCore
import Foundation
import Accelerate


public class PointCloudController {
    @objc dynamic fileprivate(set) internal var pointCloud = [PointCloudModel]()
    static let shared = PointCloudController()
    let plot = Plot.shared
    let inMin = 0
    
    let outMin:CGFloat = 0
    let outMax:CGFloat = 1

    
    
    func map(x:CGFloat, inMin:CGFloat , inMax:CGFloat) -> CGFloat {
      return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }
    


}
