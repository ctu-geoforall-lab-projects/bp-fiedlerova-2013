/***************************************************************************
    qgsconflateprovider.h

    QgsConflateProvider
    functionality for plugin Conflate

    ---------------------
    begin                : April 2013
    copyright            : (C) 2013 by Tereza Fiedlerová
    email                : tfiedlerova dot at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSCONFLATEPROVIDER_H
#define QGSCONFLATEPROVIDER_H

// QGIS includes
#include <qgsmaplayer.h>
#include <qgsvectorlayer.h>

// Local includes
#include "geoc.h"

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::alg;
using namespace geoc::edit;
using namespace geoc::tin;


/** Class providing conflate functionality of plugin */

class QgsConflateProvider
{
public:
    QgsConflateProvider();
    ~QgsConflateProvider();

    /** Sets reference layer mRefLayer.
      */
    void setRefVectorLayer( QgsVectorLayer *ref ){ mRefLayer = ref; }

    /** Sets subject layer mSubLayer.
      */
    void setSubVectorLayer( QgsVectorLayer *sub ){ mSubLayer = sub; }

    /** Return new layer - modified subject layer.
      */
    QgsVectorLayer* getNewVectorLayer(){ return mNewLayer; }

    /** Return number of features to be processed
      */
    long getFNumber(){ return mGeosSub.size(); }

    /** Create empty layer to the given uri.
      * @param uri Uri of new empty layer.
      * @true if creation was succesfull
      */
    bool createEmptyLayer( QString uri );

    /** Creates new uri to according to given parameters.
      * @param path Path to the directory where new file should be created ended by base name of new file.
      * @param rank Number used for file name.
      * @return new uri to create new file
      */
    QString newUri( QString path, int &rank);

    /** Check if file (source) exists.
      * @param source Path to the file.
      * @return true if there is file at this source
      */
    bool fileExists( QString source );

    /** Copy subject layer to the mNewLayer.
     *  @return true if copied layer is valid
     */
    bool copyLayer( QString uri );

    /** Check if layers are of the same type.
      */
    bool checkLayersType();

    /** Transfers qgis geometry to geos.
     *  @param theLayer Layer which geometry has to be transfered.
     */
    void transferGeometrytoGeos( bool isRefLayer );

    /** Transfers geos geometry to qgis.
      * @return true if transfering was succesfull
     */
    bool transferGeometryFromGeos( );

    /** Snap vertices */
    void vertexSnap();

    /** Coverage alignment */
    void align();

    /** Line matching */
    void lineMatch();

    /** Set distance tolerance for conflation.
      */
    void setTolDistance( double dist ){ mTolDistance = dist; }

    /** Set if geometry correction is wanted */
    void setRepair( bool repair ){ correct = repair; }

    void setMatchCriterium( double tol ){ mMatchTol = tol; }

    /** Get text protocol about computation
      */
    QString getProtocol() { return mProtocol; }

    /** Write protocol about computation and given invalid geometries.
      */
    void writeProtocol( const vector<int> &invalids, QString method );

private:

    QgsVectorLayer *mRefLayer;
    QgsVectorLayer *mSubLayer;
    QgsVectorLayer *mNewLayer;
    TGeomLayer mGeosSub;
    TGeomLayer mGeosRef;
    TGeomLayer mGeosNew;
    double mTolDistance;
    double mMatchTol;
    QString mProtocol;
    bool correct;

};

#endif // QGSCONFLATEPROVIDER_H
