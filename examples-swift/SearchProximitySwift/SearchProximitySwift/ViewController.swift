//
//  ViewController.swift
//  SearchProximitySwift
//
//  Created by Juan Kruger on 31/01/18.
//  Copyright © 2018 LocusLabs. All rights reserved.
//

import UIKit

class ViewController: UIViewController, LLAirportDatabaseDelegate, LLFloorDelegate, LLMapViewDelegate, LLSearchDelegate {

    // Vars
    var airportDatabase:    LLAirportDatabase!
    var airport:            LLAirport?
    var floor:              LLFloor?
    var mapView:            LLMapView?
    var search:             LLSearch?
    
    // MARK: Lifecycle
    override func viewDidLoad() {
        
        super.viewDidLoad()
        
        // Initialize the LocusLabs SDK with the accountId provided by LocusLabs
        LLLocusLabs.setup().accountId = "A11F4Y6SZRXH4X"
        
        // Get an instance of the LLAirportDatabase, register as its delegate and load the venue LAX
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
    
    func createCircle(center: LLLatLng, floor: String, radius: Float, color: UIColor) {
        
        let circle = LLCircle(center: center, radius: radius as NSNumber)
        circle?.fillColor = color
        circle?.floorView = mapView?.getFloorView(forId: floor)
    }
    
    // MARK: Delegates - LLAirportDatabase
    
    func airportDatabase(_ airportDatabase: LLAirportDatabase!, airportLoadFailed venueId: String!, code errorCode: LLDownloaderError, message: String!) {
        
        // Handle failures here
    }
    
    func airportDatabase(_ airportDatabase: LLAirportDatabase!, airportLoaded airport: LLAirport!) {
        
        self.airport = airport
        
        // Get a list of buildings in this airport and load the first one
        if let buildingInfo = self.airport?.listBuildings().first as? LLBuildingInfo {
            
            let building = self.airport?.loadBuilding(buildingInfo.buildingId)
            
            // Get a list of floors for the building and load the first one
            if let floorInfo = building?.listFloors().first as? LLFloorInfo {
                
                floor = building?.loadFloor(floorInfo.floorId)
                
                // Set the floor delegate and load its map - mapLoaded is called when loading is complete
                floor?.delegate = self
                floor?.loadMap()
                
                // Get a search instance and register as its delegate
                search = airport.search()
                search?.delegate = self;
            }
        }
    }

    // MARK: Delegates - LLFloor
    func floor(_ floor: LLFloor!, mapLoaded map: LLMap!) {
        
        mapView?.map = map
    }
    
    // MARK: Delegates - LLMapView
    
    func mapViewDidClickBack(_ mapView: LLMapView!) {
        
        // The user tapped the "Cancel" button while the map was loading. Dismiss the app or take other appropriate action here
    }
    
    func mapViewReady(_ mapView: LLMapView!) {
        
        search?.proximitySearch(withTerms: ["Starbucks"], floorId: "lax-south-departures", lat: NSNumber(value:33.94221), lng: NSNumber(value:-118.402057))
    }
    
    // MARK: Delegates - LLPOIDatabase
    
    func poiDatabase(_ poiDatabase: LLPOIDatabase!, poiLoaded poi: LLPOI!) {
        
        // We only want to mark "Food" results on the map that fall in the "Eat" category
        if poi.category == "eat" {
            
            let position = poi.position
            createCircle(center: position!.latLng, floor: position!.floorId, radius: 10, color: UIColor.blue)
        }
    }
    
    // MARK: Delegates - LLSearch
    
    func proximitySearch(withTerms search: LLSearch!, results searchResults: LLSearchResults!) {
        
        for searchResult in searchResults.results as! [LLSearchResult] {
            
            let position = searchResult.position
            createCircle(center: position!.latLng, floor: position!.floorId, radius: 10, color: UIColor.red)
        }
    }
}

