// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

package com.eegeo.searchmenu;

public class SearchMenuViewJniMethods
{
    public static native void HandleClosed(long nativeCallerPointer);
    
    public static native void SelectedItem(long nativeCallerPointer, int itemIndex);
}
