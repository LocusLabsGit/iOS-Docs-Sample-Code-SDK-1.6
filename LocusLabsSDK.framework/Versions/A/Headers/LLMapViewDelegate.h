//
//  LLMapViewDelegate.h
//  LocusLabsSDK
//
//  Created by Samuel Ziegler on 7/22/14.
//  Copyright (c) 2014 LocusLabs. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LLMapView;
@class LLMarker;
@class LLLatLng;
@class LLPosition;
@class LLPoint;

typedef enum _LLMapViewMode
{
    LLMapViewModeNormal = 1,
    LLMapViewModeNavigation
} LLMapViewMode;

typedef NS_ENUM(NSInteger, LLMapViewMarkerType)
{
    LLMapViewMarkerTypeSelected = 1,
    LLMapViewMarkerTypeUnselected
};

typedef NS_ENUM(NSInteger, LLMapViewPresentationContext)
{
	LLMapViewPresentationContextAlert,
	LLMapViewPresentationContextCheckIn,
	LLMapViewPresentationContextVouchers
};

@protocol LLMapViewDelegate <NSObject>

@optional

- (void)mapViewReady:(LLMapView *)mapView;

/**
 *  Is called by the LLMapView whenever a marker on the map is tapped.
 *
 *  @param mapView the mapView that generated the event
 *  @param marker  the marker that was tapped
 *
 *  @return return TRUE if the event has been handled, FALSE if mapView should continue with it's default processing.
 */
- (BOOL)mapView:(LLMapView *)mapView didTapMarker:(LLMarker *)marker;

/**
 * Called when a draggable marker begins a drag.
 *
 *  @param mapView the mapView that generated the event
 *  @param marker  the marker that was tapped
 */
- (void)mapView:(LLMapView *)mapView didBeginDraggingMarker:(LLMarker *)marker;

/**
 * Called when a draggable marker ends a drag.
 *
 *  @param mapView the mapView that generated the event
 *  @param marker  the marker that was tapped
 */
- (void)mapView:(LLMapView *)mapView didEndDraggingMarker:(LLMarker *)marker;

/**
 * Called when a draggable marker is being dragged.
 *
 *  @param mapView the mapView that generated the event
 *  @param marker  the marker that was tapped
 */
- (void)mapView:(LLMapView *)mapView didDragMarker:(LLMarker *)marker;

/**
 *  Called whenever a point on the map not containing a marker is tapped.
 *
 *  @param mapView the mapView that generated the event
 *  @param latLng  the point where the tap event occurred
 */
- (void)mapView:(LLMapView *)mapView didTapAtLatLng:(LLLatLng *)latLng;

/**
 *  Called whenever a point on the map not containing a marker is tapped.
 *
 *  @param mapView the mapView that generated the event
 *  @param position the position where the tap event occurred
 */
- (void)mapView:(LLMapView *)mapView didTapAtPosition:(LLPosition *)position;

/**
 *  Called whenever a POI on the map has been tapped.
 *
 *  @param mapView the mapView that generated the event
 *  @param poiId   the id of the POI that was tapped
 */
- (BOOL)mapView:(LLMapView *)mapView didTapPOI:(NSString *)poiId;

/**
 *	Called just before a POI is about to display to allow additional views/controls to be displayed beside the Navigate button.
 *	@note Returning nil or an empty array are both valid ways of ignoring this method for some POIs and not others.
 *
 *	@param mapView	the mapView that generated the event
 *	@param poiId	the id of the POI that will be displayed
 *
 *	@returns An array of UIView objects (usually UIButtons or other UIControls) to be displayed in addition to the Navigate button.
 */
- (NSArray *)mapView:(LLMapView *)mapView additionalViewsForPOI:(NSString *)poiId;

- (void)mapViewDidClickBack:(LLMapView *)mapView;
- (void)mapView:(LLMapView *)mapView floorChanged:(NSString *)floorId;
- (void)mapView:(LLMapView *)mapView modeChanged:(LLMapViewMode)mode;

/**
 * Provide an alternate marker icon URL for the LLMapView to use for specific POIs.
 * @param mapView the mapView that generated the event
 * @param poiId   the id of the POI
 * @param type the type of marker, selected or unselected
 */
- (NSString *)mapView:(LLMapView *)mapView markerIconUrlForPOI:(NSString *)poiId type:(LLMapViewMarkerType)type;

/**
 * Provide an alternate anchor point for the LLMapView to use for specific POIs.  If an alternate icon url is also provided,
 * then the anchor will default to the bottom center of the icon.
 * @param mapView the mapView that generated the event
 * @param poiId   the id of the POI
 * @param type the type of marker being displayed, selected or unselected
 */
- (LLPoint *)mapView:(LLMapView *)mapView markerAnchorForPOI:(NSString *)poiId type:(LLMapViewMarkerType)type;

- (BOOL)mapView:(LLMapView *)mapView isSponsoredSearchResult:(NSString *)poiId;

/**
 * Called whenever the map view needs to present a view controller modally.
 * In most cases, the view controller returned should be the same that owns the MapView.
 * @param mapView the mapView that generated the event
 * @param context a flag indicating what context/event is asking for the presentation controller, for cases where a different controller might be needed
 * @returns A view controller upon which a presentation will be made.
 */
- (UIViewController *)presentingControllerForMapView:(LLMapView *)mapView forContext:(LLMapViewPresentationContext)context;

/**
 * @brief This method is called when the map is about to display any featured places.
 * The @c places array may be empty or already have @c LLPlace objects in it. Either way,
 * this method should return the array with any desired modifications, such as additional items
 * or rearranged order.
 * @discussion In addition, an @c[NSNull @c null] object as the first item in the array
 * has special meaning; it will prevent the first button from being shown in the list and only display
 * a single menu item.
 * @param mapView the mapView that generated the event
 * @param places the array of @c LLPlace objects
 * @returns a modified version of the @c places array, a new array of @c LLPlace objects, or an empty array
 */
- (NSArray *)mapView:(LLMapView *)mapView willPresentPlaces:(NSArray *)places;

/**
 * @brief This method is called when the search panel is about to display suggestions.
 * The @c suggestions array may be empty or already have strings in it.
 * These strings will simply act as search terms when selected. Either way,
 * this method should return the array with any desired modifications,
 * such as additional items, removed items, or rearranged order.
 * @param mapView the mapView that generated the event
 * @param suggestions the array of strings
 * @returns a modified version of the @c suggestions array, a new array of strings, or an empty array
 */
- (NSArray<NSString*> *)mapView:(LLMapView *)mapView willPresentSearchSuggestions:(NSArray<NSString*> *)suggestions;

@end
