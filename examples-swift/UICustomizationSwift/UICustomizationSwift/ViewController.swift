//
//  ViewController.swift
//  UICustomizationSwift
//
//  Created by Juan Kruger on 31/01/18.
//  Copyright © 2018 LocusLabs. All rights reserved.
//

import UIKit

class ViewController: UIViewController, LLAirportDatabaseDelegate, LLFloorDelegate, LLMapViewDelegate {

    // Vars
    var airportDatabase:    LLAirportDatabase!
    var airport:            LLAirport?
    var floor:              LLFloor?
    var mapView:            LLMapView?
    
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
    func themeWithCustomBottomBar() -> LLTheme {
        
        let themeBuilder = LLThemeBuilder(theme: LLTheme.default())
        themeBuilder.setProperty("MapView.BottomBar.backgroundColor", value: UIColor.orange)
        themeBuilder.setProperty("MapView.BottomBar.Button.Title.textColor", value: UIColor.black)
        
        return themeBuilder.theme
    }
    
    func themeWithCustomFont(customFont: UIFont) -> LLTheme {
        
        let themeBuilder = LLThemeBuilder(theme: LLTheme.default())
        themeBuilder.setProperty("fonts.normal", value: customFont)
        
        return themeBuilder.theme
    }
    
    // MARK: Delegates - LLAirportDatabase
    
    func airportDatabase(_ airportDatabase: LLAirportDatabase!, airportLoadFailed venueId: String!, code errorCode: LLDownloaderError, message: String!) {
        
        // Handle failures here
    }
    
    // Implement the airportLoaded delegate method
    func airportDatabase(_ airportDatabase: LLAirportDatabase!, airportLoaded airport: LLAirport!) {
        
        self.airport = airport
        
        /// Get a list of buildings in this airport and load the first one
        if let buildingInfo = self.airport?.listBuildings().first as? LLBuildingInfo {
            
            let building = self.airport?.loadBuilding(buildingInfo.buildingId)
            
            // Get a list of floors for the building and load the first one
            if let floorInfo = building?.listFloors().first as? LLFloorInfo {
                
                floor = building?.loadFloor(floorInfo.floorId)
                
                // Set the floor delegate and load its map - mapLoaded is called when loading is complete
                floor?.delegate = self
                floor?.loadMap()
            }
        }
    }

    // MARK: Delegates - LLFloor
    
    func floor(_ floor: LLFloor!, mapLoaded map: LLMap!) {
        
        mapView?.map = map
        
        // Change the font
        //mapView?.theme = themeWithCustomFont(customFont: UIFont(name: "American Typewriter", size: 12)!)
        
        // Set a custom back button title
        //mapView?.setBackButtonText(NSLocalizedString("Back", comment: ""))
        
        // Change the search bar background color
        //mapView?.setSearchBarBackgroundColor(UIColor.orange)
        
        // Change the bottom bar background and button title colors
        mapView?.theme = themeWithCustomBottomBar()
    }
    
    // MARK: Delegates - LLMapView
    
    func mapViewDidClickBack(_ mapView: LLMapView!) {
        
        // The user tapped the "Cancel" button while the map was loading. Dismiss the app or take other appropriate action here
    }
    
    func mapViewReady(_ mapView: LLMapView!) {
        
        // The map is ready to be used in calls e.g. zooming, showing poi, etc.
    }
}

