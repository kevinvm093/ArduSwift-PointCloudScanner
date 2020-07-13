//
//  SCNVector3+Ext.swift
//  LidarScanner
//
//  Created by Kevin Vallejo on 7/4/20.
//  Copyright © 2020 Kevin Vallejo. All rights reserved.
//

import Foundation
import SceneKit

extension SCNVector3 {
    
    func magnitude() -> CGFloat {
        let mx = x*x
        let my = y*y
        let mz = z*z
        return sqrt(mx + my + mz)
    }
    
    func getAngle() -> CGFloat {
        return acos(y/magnitude())
    }
  
}



