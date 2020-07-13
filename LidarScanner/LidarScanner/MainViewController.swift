//
//  MainViewController.swift
//  LidarScanner
//
//  Created by Kevin Vallejo on 6/25/20.
//  Copyright © 2020 Kevin Vallejo. All rights reserved.
//

import SceneKit
import QuartzCore
import Cocoa

class MainViewController: NSViewController {
    
    let plot = Plot.shared
    let scene = SCNScene()
    // let uartHandler = UARThandler()
    let pointCloud = PointCloud.shared
    private var dataObserver: NSKeyValueObservation?

    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        addGesture()
        generateLight()
        generateCamera()
        configureScene()
        
        dataObserver = pointCloud.observe(\.vectors) { [weak self] (controller, change) in
            self?.draw()
        }
        NSEvent.addLocalMonitorForEvents(matching: .keyDown) {
            if self.keyDown(with: $0) {
               return nil
            } else {
               return $0
            }
        }
        
        
       
    }
    


      func keyDown(with event: NSEvent) -> Bool {
        let cameraNode = scene.rootNode.childNode(withName: "camera", recursively: true)
        if (event.keyCode == 123) {
            
            cameraNode!.transform = SCNMatrix4Rotate(cameraNode!.transform, -.pi/12, 0, 1, 0)
        } else if (event.keyCode == 124) {
            cameraNode!.transform = SCNMatrix4Rotate(cameraNode!.transform, -.pi/12, 1, 0, 0)
        } else if (event.keyCode == 125) {
            cameraNode!.transform = SCNMatrix4Rotate(cameraNode!.worldTransform, .pi/12, 0, 1, 1)
        }
        else if (event.keyCode == 126) {
             cameraNode!.transform = SCNMatrix4Rotate(cameraNode!.worldTransform, .pi/12, 1, 1, 0)
        }
        return true
    }
    
    override func viewDidAppear() {
        
         DispatchQueue.global(qos: .background).async { [weak self] in
                   guard let self = self else {
                       return
                   }
            DispatchQueue.main.async { [weak self] in
                self?.pointCloud.readFile()
            }
                   
               }
    }
    
    /// Creates a sphere node that gets added into view. Together every sphere will add up to draw a 3D image.
    /// - Parameters:
    ///   - data: DataModel in which contains x y z cartesian coordinates as well as their spherical polor counter parts.
    /// - Returns: nothing.
    
    func draw() {
        
        let data = pointCloud.vectors.last!
        let geometry = SCNSphere(radius: plot.sphereRadius)
        let material = SCNMaterial()
        
        material.diffuse.contents = pointCloud.colorHSB
        let theta = Int((acos(data.z/data.magnitude()) * 180 / .pi).rounded(.toNearestOrAwayFromZero))
      
        
        geometry.materials = [material]
        let geometryNode = SCNNode(geometry: geometry)
        geometryNode.position = data
        

        
        
        if theta % 2 == 0 {
            geometryNode.transform = SCNMatrix4Rotate(geometryNode.worldTransform, -.pi/15, 0, 0, 1)
        } else {
            geometryNode.transform = SCNMatrix4Rotate(geometryNode.worldTransform, .pi/15, 0, 0, 1)
        }
        
        geometryNode.transform = SCNMatrix4Invert(geometryNode.transform )
        scene.rootNode.addChildNode(geometryNode)
    }
}


