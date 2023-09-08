import kivy
kivy.require('2.0.0')

from kivy.app import App
from kivy.uix.dropdown import DropDown
from kivy.uix.button import Button
from kivy.lang import Builder
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout

# from kivy.uix.boxlayout import BoxLayout
# from kivy.uix.label import Label
# from kivy.uix.popup import Popup
# from kivy.uix.filechooser import FileChooserListView

Builder.load_string("""
<CustomDropDown>
    Button: 
        text: "Manual"
        size_hint_y: None
        height: 40
        on_release: root.select("Manual")
    Button: 
        text: "Pick and Place"
        size_hint_y: None
        height: 40
        on_release: root.select("Pick and Place")

    Button: 
        text: "Shape Drawing"
        size_hint_y: None
        height: 40
        on_release: root.select("Shape Drawing")

""")

class CustomDropDown(DropDown):
    pass

class GUI(App):
    def build(self):
        self.title = 'EEE 318 Arm Controller'
        
        layout = BoxLayout(orientation='vertical')

        label = Label(text="Arm Control Software", size_hint=(1, 0.1))

        mainbutton = Button(text= "Select Mode",
                            size_hint=(0.4,0.1),
                            pos_hint={'center_x':0.5,'center_y':0})
        
        mode = CustomDropDown()

        mainbutton.bind(on_release = mode.open)

        mode.bind(on_select = lambda instance,x : setattr(mainbutton, 'text', x))
        
        layout.add_widget(label)
        layout.add_widget(mainbutton)

        return layout
            

if __name__ == '__main__':
    GUI().run()
