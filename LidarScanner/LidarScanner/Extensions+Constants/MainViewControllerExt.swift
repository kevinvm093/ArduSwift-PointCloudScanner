//
//  LidarViewControllerExt.swift
//  LidarScanner
//
//  Created by Kevin Vallejo on 6/27/20.
//  Copyright © 2020 Kevin Vallejo. All rights reserved.
//

import Foundation
import SceneKit
import QuartzCore

extension MainViewController {

    func generateCamera() {
        
        // create and add a camera to the scene
        cameraNode.name = "camera"
        cameraNode.camera = SCNCamera()
        cameraNode.position = SCNVector3(x: 13, y: 38, z: 68)
        cameraNode.camera?.zFar = 350.0
        
        self.scene.rootNode.addChildNode(cameraNode)
    }
    
    func generateLight() {
        
        // create and add a light to the scene
        let lightNode = SCNNode()
        lightNode.light = SCNLight()
        lightNode.light!.type = .omni
        lightNode.position = SCNVector3(x: 0, y: 0, z: 0)
        scene.rootNode.addChildNode(lightNode)
        
        // create and add an ambient light to the scene
        let ambientLightNode = SCNNode()
        ambientLightNode.light = SCNLight()
        ambientLightNode.light!.type = .ambient
        ambientLightNode.light!.color = NSColor.lightGray
        scene.rootNode.addChildNode(ambientLightNode)
    }
    
    func configureScene() {
        
        let scnView = self.view as! SCNView
        scnView.scene = scene
        scnView.allowsCameraControl = true
        scnView.showsStatistics = false
        scnView.backgroundColor = NSColor.init(calibratedRed: 27/255, green: 29/255, blue: 31/255, alpha: 1.0)
    }
}
