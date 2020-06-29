//
//  MainViewController.swift
//  LidarScanner
//
//  Created by Kevin Vallejo on 6/25/20.
//  Copyright © 2020 Kevin Vallejo. All rights reserved.
//

import SceneKit
import QuartzCore

class MainViewController: NSViewController {
    
    let plot = Plot.shared
    let scene = SCNScene()
    let uartHandler = UARThandler()
    let dataParser = DataParser.shared
    private var dataObserver: NSKeyValueObservation?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        addGesture()
        generateLight()
        generateCamera()
        configureScene()
        
        dataObserver = dataParser.observe(\.dataPoints) { [weak self] (controller, change) in
            self?.draw(data: controller.dataPoints.last!)
        }
    }
    
    /// Creates a sphere node that gets added into view. Together every sphere will add up to draw a 3D image.
    /// - Parameters:
    ///   - data: DataModel in which contains x y z cartesian coordinates as well as their spherical polor counter parts.
    /// - Returns: nothing.
    
    func draw(data:DataModel) {
        
        let geometry = SCNSphere(radius: plot.sphereRadius)
        let material = SCNMaterial()
        
        if data.zPoint < plot.far && data.yPoint > plot.near {
            
            material.diffuse.contents = NSColor.blue
            
        } else if data.zPoint < plot.far && data.yPoint < plot.near {
            
            material.diffuse.contents = NSColor.green
            
        } else if data.zPoint > plot.far && data.zPoint < plot.veryFar {
            
            material.diffuse.contents = NSColor.yellow
        }
                
        geometry.materials = [material]
        let geometryNode = SCNNode(geometry: geometry)
        geometryNode.position = data.sphereVector
        geometryNode.scale = SCNVector3(plot.scale, plot.scale, plot.scale)
        scene.rootNode.addChildNode(geometryNode)
    }
    
}
