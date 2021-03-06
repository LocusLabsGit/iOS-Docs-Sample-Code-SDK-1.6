//
//  ViewController.swift
//  DirectionsDrawSwift
//
//  Created by Juan Kruger on 31/01/18.
//  Copyright © 2018 LocusLabs. All rights reserved.
//

import UIKit

class ViewController: UIViewController, LLAirportDatabaseDelegate, LLFloorDelegate, LLMapViewDelegate, LLAirportDelegate {

    // Vars
    var airportDatabase:    LLAirportDatabase!
    var airport:            LLAirport?
    var floor:              LLFloor?
    var mapView:            LLMapView?
    var navPoint:           LLNavPoint?
    
    // MARK: Lifecycle
    
    override func viewDidLoad() {
        
        super.viewDidLoad()
        
        // Initialize the LocusLabs SDK with the accountId provided by LocusLabs
        LLLocusLabs.setup().accountId = "A11F4Y6SZRXH4X"
        
        // Get an instance of the LLAirportDatabase and register as its delegate
        airportDatabase = LLAirportDatabase()
        airportDatabase.delegate = self
        
        // Create a new LLMapView, register as its delegate and add it as a subview
        mapView = LLMapView()
        mapView!.delegate = self
        view.addSubview(mapView!)
        
        // Set the mapview's layout constraints
        mapView!.translatesAutoresizingMaskIntoConstraints = false
        mapView!.leftAnchor.constraint(equalTo: view.leftAnchor, constant: 0).isActive = true
        mapView!.rightAnchor.constraint(equalTo: view.rightAnchor, constant: 0).isActive = true
        mapView!.topAnchor.constraint(equalTo: view.topAnchor, constant: 0).isActive = true
        mapView!.bottomAnchor.constraint(equalTo: view.bottomAnchor, constant: 0).isActive = true
        
        airportDatabase.loadAirport("lax")
    }
    
    // MARK: Custom
    func createCircleCenteredAt(latLng: LLLatLng!, onFloor floorId: String!, radius: NSNumber, color: UIColor!) {
        
        let circle = LLCircle(center: latLng, radius: radius)
        circle?.fillColor = color
        circle?.floorView = mapView?.getFloorView(forId: floorId)
    }
    
    func drawRoute(waypoints: [LLWaypoint], startFloor: String!) {
        
        let path = LLMutablePath()
        for waypoint in waypoints {
            
            // Add this latLng to the LLPath
            path.add(waypoint.latLng)
            
            // Add a black circle at the destination
            if waypoint.isDestination == true {
                
                createCircleCenteredAt(latLng: waypoint.latLng, onFloor: waypoint.floorId, radius: 5, color: UIColor.black)
            }
        }
        
        // Create a new LLPolyline object and set its path
        let polyLine = LLPolyline()
        polyLine.path = path
        polyLine.floorView = mapView?.getFloorView(forId: startFloor)
    }
    
    func showSampleRoute() {
        
        let point1LatLon = LLLatLng(lat: 33.940627, lng: -118.401892)
        let point2LatLon = LLLatLng(lat: 33.9410700, lng: -118.399598)
        
        let point1 = LLPosition(floor: floor, latLng: point1LatLon)
        let point2 = LLPosition(floor: floor, latLng: point2LatLon)
        
        airport?.navigate(from: point1, to: point2)
    }

    // MARK: Delegates - LLAirport
    
    func airport(_ airport: LLAirport!, navigationPath: LLNavigationPath!, from startPosition: LLPosition!, toDestinations destinations: [Any]!) {
        
        drawRoute(waypoints: navigationPath.waypoints as! [LLWaypoint], startFloor: startPosition.floorId)
    }
    
    // MARK: Delegates - LLAirportDatabase
    
    func airportDatabase(_ airportDatabase: LLAirportDatabase!, airportLoadFailed venueId: String!, code errorCode: LLDownloaderError, message: String!) {
        
        // Handle failures here
    }
    
    // Implement the airportLoaded delegate method
    func airportDatabase(_ airportDatabase: LLAirportDatabase!, airportLoaded airport: LLAirport!) {
        
        self.airport = airport
        self.airport?.delegate = self
        
        let building = self.airport?.loadBuilding("lax-south")
        floor = building?.loadFloor("lax-south-departures")
        
        // Set the floor delegate and load its map - mapLoaded is called when loading is complete
        floor?.delegate = self
        floor?.loadMap()
    }

    // MARK: Delegates - LLFloor
    
    func floor(_ floor: LLFloor!, mapLoaded map: LLMap!) {
        
        mapView!.map = map
    }
    
    // MARK: Delegates - LLMapView
    
    func mapViewDidClickBack(_ mapView: LLMapView!) {
        
        // The user tapped the "Cancel" button while the map was loading. Dismiss the app or take other appropriate action here
    }
    
    func mapViewReady(_ mapView: LLMapView!) {
        
        // Pan/zoom the map after selecting lax-south-Departured
        mapView.levelSelected("lax-south-departures")
        mapView.mapCenter = LLLatLng(lat: 33.941384, lng: -118.402057)
        mapView.mapRadius = 190.0
        
        showSampleRoute()
    }
}

