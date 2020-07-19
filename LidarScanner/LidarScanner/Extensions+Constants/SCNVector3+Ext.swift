//
//  SCNVector3+Ext.swift
//  LidarScanner
//
//  Created by Kevin Vallejo on 7/4/20.
//  Copyright © 2020 Kevin Vallejo. All rights reserved.
//

import Foundation
import SceneKit

extension simd_float3 {
    func getAngle() -> CGFloat {
        return CGFloat(acos(y/simd_length(self)))
    }
}


