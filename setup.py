from distutils.core import setup, Extension

module1 = Extension("scene",
                    libraries = ["SDL"],
                    sources = ["scene.c"])

setup (name = "scene",
       version = "0.1",
       description = "Python bindings for SDL",
       author = "Victor Kindhart",
       author_email = "vikkindhart@gmail.com",
       long_description = """\
Python bindings for SDL allowing to
run games made with `scene' library.
""",
       ext_modules = [module1])
