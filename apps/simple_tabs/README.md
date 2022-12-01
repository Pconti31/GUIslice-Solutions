
# Sample Tabs Application

Demonstrates using the Extended Element XTabBtn control.

The point of this control is to simplify tab handling.

<p>
Without this control you need write your own tracking code,
turning on highlighting your buttons yourself. Putting in code to jump
to another page, etc... This increases in complexity as you add tabs.
</p> 

<p>
The sample is set for Arduino using AdaFruit_GFX library and a standard
320x240 pixel display but you can use the GUIsliceBuilder to set these
to any supported platform. Simply use the PROJECT_OPTIONS tab inside 
the builder and choose your IDE, screen size and graphics package.
</p> 

<p>
Using this control you layout your project using standard Text Buttons
most likely on your Base page and at runtime call `redefineButtonAsTab`.
The GUIsliceBuilder will do the memory allocation for you and will 
leave your `redefineButtonAsTab` unharmed as long as they are placed outside
of the various tags used by the builder.  The best place is after the
InitGUIslice_gen() call inside setup() routine of your ino or main.cpp file.
</p>

Remember that you when you define your Text Buttons to set the reference name:

ElementRef = {some meaningful name like "m_pElemTabMain"}

The redefineButtonAsTab takes the following parameters:
```
///
/// Redefine a TabBtn Element from a Text Button Element
///
/// \param[in]  pGui:        Pointer to GUI
/// \param[in]  pElemRef:    Pointer to a TextButton element reference
/// \param[in]  nPage:       Page ID where this element is attached
/// \param[in]  nJumpPage:   Jump to this Page ID when selected
/// \param[in]  bState:      Starting state true=selected, false=not selected
///
/// \return none
```
