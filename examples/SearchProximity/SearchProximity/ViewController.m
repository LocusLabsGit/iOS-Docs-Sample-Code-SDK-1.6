//
//  ViewController.m
//  SearchProximity
//
//  Created by Juan Kruger on 31/01/18.
//  Copyright © 2018 LocusLabs. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@property (nonatomic, strong) LLAirport         *airport;
@property (nonatomic, strong) LLAirportDatabase *airportDatabase;
@property (nonatomic, strong) LLFloor           *floor;
@property (nonatomic, weak)   LLMapView         *mapView;
@property (nonatomic, strong) LLSearch          *search;

- (void)createCircleCenteredAt:(LLLatLng*)latLng onFloor:(NSString*)floorId withRadius:(NSNumber*)radius andColor:(UIColor*)color;

@end

@implementation ViewController

#pragma mark Lifecycle

- (void)viewDidLoad {
    
    [super viewDidLoad];
    
    // Initialize the LocusLabs SDK with the accountId provided by LocusLabs
    [LLLocusLabs setup].accountId = @"A11F4Y6SZRXH4X";
    
    // Get an instance of the LLAirportDatabase and register as its delegate
    self.airportDatabase = [LLAirportDatabase airportDatabase];
    self.airportDatabase.delegate = self;
    
    // Create a new LLMapView, register as its delegate and add it as a subview
    LLMapView *mapView = [[LLMapView alloc] init];
    self.mapView = mapView;
    self.mapView.delegate = self;
    [self.view addSubview:mapView];
    
    // Set the mapview's layout constraints
    mapView.translatesAutoresizingMaskIntoConstraints = NO;
    [mapView.leftAnchor constraintEqualToAnchor:self.view.leftAnchor].active = YES;
    [mapView.rightAnchor constraintEqualToAnchor:self.view.rightAnchor].active = YES;
    [mapView.topAnchor constraintEqualToAnchor:self.view.topAnchor].active = YES;
    [mapView.bottomAnchor constraintEqualToAnchor:self.view.bottomAnchor].active = YES;
    
    // Load the venue LAX
    [self.airportDatabase loadAirport:@"lax"];
}

#pragma mark Custom

- (void)createCircleCenteredAt:(LLLatLng*)latLng onFloor:(NSString*)floorId withRadius:(NSNumber*)radius andColor:(UIColor*)color {
    
    LLCircle *circle = [LLCircle circleWithCenter:latLng radius:radius];
    [circle setFillColor:color];
    circle.floorView = [self.mapView getFloorViewForId:floorId];
}

#pragma mark Delegates - LLAirportDatabase

- (void)airportDatabase:(LLAirportDatabase *)airportDatabase airportLoadFailed:(NSString *)venueId code:(LLDownloaderError)errorCode message:(NSString *)message {
    
    // Handle failures here
}

- (void)airportDatabase:(LLAirportDatabase *)airportDatabase airportLoaded:(LLAirport *)airport {
    
    self.airport = airport;
    
    // Get a list of buildings in this airport and load the first one
    LLBuildingInfo *buildingInfo = [self.airport listBuildings][0];
    LLBuilding *building  = [self.airport loadBuilding:buildingInfo.buildingId];
    
    // Get a list of floors for the building and load the first one
    LLFloorInfo *floorInfo = [building listFloors][0];
    self.floor = [building loadFloor:floorInfo.floorId];
    
    // Set the floor delegate and load its map - mapLoaded is called when loading is complete
    self.floor.delegate = self;
    [self.floor loadMap];
    
    // Get a search instance and register as its delegate
    self.search = [self.airport search];
    self.search.delegate = self;
}

#pragma mark Delegates - LLFloor

- (void)floor:(LLFloor *)floor mapLoaded:(LLMap *)map {
    
    self.mapView.map = map;
}

#pragma mark Delegates - LLMapView

- (void)mapViewDidClickBack:(LLMapView *)mapView {
    
    // The user tapped the "Cancel" button while the map was loading. Dismiss the app or take other appropriate action here
}

- (void)mapViewReady:(LLMapView *)mapView {
    
    // Find the nearest Starbucks to gate 60
    [self.search proximitySearchWithTerms:[NSArray arrayWithObjects:@"Starbucks",nil]
                                  floorId:@"lax-south-departures"
                                      lat:@33.94221
                                      lng:@-118.402057];
}

#pragma mark Delegates - LLSearch

- (void)proximitySearchWithTerms:(LLSearch *)search results:(LLSearchResults *)searchResults {
    
    for (LLSearchResult *searchResult in searchResults.results) {
        
        LLPosition *p = searchResult.position;
        [self createCircleCenteredAt:p.latLng onFloor:p.floorId withRadius:@10 andColor:[UIColor redColor]];
    }
}

@end
