# Dummy sound module

class Player(object):
    def __init__(*args, **kwargs):
        self.current_time = 0
        self.duration = 0
        self.number_of_loops = 0
        self.volume = 0

    def pause(*arsg, **kwargs):
        pass

    def play(*arsg, **kwargs):
        pass

    def stop(*arsg, **kwargs):
        pass

def load_effect(*arsg, **kwargs):
    pass

def play_effect(*arsg, **kwargs):
    pass

def set_volume(*arsg, **kwargs):
    pass

def stop_effect(*arsg, **kwargs):
    pass
