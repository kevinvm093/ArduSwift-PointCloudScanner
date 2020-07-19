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
    let cameraNode = SCNNode()
    // let uartHandler = UARThandler()
    let pointCloud = PointCloud.shared
    private var dataObserver: NSKeyValueObservation?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        generateLight()
        generateCamera()
        configureScene()
        
        dataObserver = pointCloud.observe(\.flag) { [weak self] (controller, true) in
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
    
    /// Creates, colors and translates a sphere node that gets added into view. Together every sphere will add up to draw a 3D image.
    /// - Parameters:
    ///   - data: none.
    /// - Returns: nothing.
    
    func draw() {
        
        let data = pointCloud.vectors.last!
        let geometry = SCNSphere(radius: plot.sphereRadius)
        
        // color
        let material = SCNMaterial()
        material.diffuse.contents = pointCloud.colorHSB
        geometry.materials = [material]
        
        // translate
        let geometryNode = SCNNode(geometry: geometry)
        geometryNode.simdPosition = geometryNode.simdConvertPosition(data, to: scene.rootNode)
        
        let theta = Int((acos(data.z/length(data)) * 180 / .pi).rounded(.toNearestOrAwayFromZero))
        if theta % 2 == 0 {
            
            geometryNode.transform = SCNMatrix4Rotate(geometryNode.worldTransform, -.pi/15, 0, 0, 1)
        } else {
            
            geometryNode.transform = SCNMatrix4Rotate(geometryNode.worldTransform, .pi/15, 0, 0, 1)
        }
        
        geometryNode.transform = SCNMatrix4Rotate(geometryNode.worldTransform, .pi, 0, 1, 1)
        geometryNode.simdTransform = simd_inverse(geometryNode.simdTransform)
        scene.rootNode.addChildNode(geometryNode)
    }
    
    ///takes in a new keyboard event and moves camera
    func keyDown(with event: NSEvent) -> Bool {
        
        if (event.keyCode == 123) { // left
            
            let rotation = simd_quatf(angle: -(Float.pi/180) , axis: normalize(simd_float3(0,1,0)))
            cameraNode.simdRotate(by: rotation, aroundTarget: simd_float3(0,0, -15))
            
        } else if (event.keyCode == 124) { //right
            
            let rotation = simd_quatf(angle: (Float.pi/180) , axis: normalize(simd_float3(0,1,0)))
            cameraNode.simdRotate(by: rotation, aroundTarget: simd_float3(0,0, -15))
            
        } else if (event.keyCode == 125) { //down
            
            let rotation = simd_quatf(angle: -(Float.pi/180) , axis: normalize(simd_float3(1,0,0)))
            cameraNode.simdRotate(by: rotation, aroundTarget: simd_float3(0,0, -50))
        }
        else if (event.keyCode == 126) {
            
            let rotation = simd_quatf(angle: (Float.pi/180) , axis: normalize(simd_float3(1,0,0)))
            cameraNode.simdRotate(by: rotation, aroundTarget: simd_float3(0,0, -50))
        }
        return true
    }
}


