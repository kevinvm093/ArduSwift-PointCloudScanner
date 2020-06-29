//
//  Config.swift
//  LidarScanner
//
//  Created by Kevin Vallejo on 6/27/20.
//  Copyright © 2020 Kevin Vallejo. All rights reserved.
//

import Foundation

// MARK: - Constants used when drawing spheres on view.
class Plot {
    
    static let shared = Plot()
    
    var sphereRadius:CGFloat {
        return 0.25
    }
    
    var far:Float {
        return 80
    }
    
    var veryFar:Float {
        return 200
    }
    
    var near: Float {
        return 25
    }
    
    var scale:Float {
        return 1.5
    }
    
}
// MARK: - Constants used when reading from file.
class File {
    
    static let shared = File()
    
    var name:String {
        return "dummyData"
    }
    
    var ext:String {
        return "txt"
    }
    
    var spaceToken:Character{
        return " "
    }
    
    var newLineToken:Character {
        return "\n"
    }
}
