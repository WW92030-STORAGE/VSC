# VSC
  
"I might have to wait, I'll never give up. I guess it's half timing, and the other half is luck..." -Michael Buble  
  
VSC (VOUGA-SHREINER-CANTH) Verified is A 3D rendering engine that is intended to be literally ported anywhere.   
Inspired by DoomGeneric with its rather novel approach to separating the backend from the frontend.  
Also self-inspired by my (NormalExisting's) other C++ backends mostly for games/clones such as The Witness and Chess.  
  
Written in C++ with the intent of minimizing the usage of C++ heavy libraries such as vector (as any self-respecting graphics engine should).  
Most data structures are defined in classes here, and mallocs are minimized. Most everything is stack allocated.  
  
This is a rasterizer. It uses computational techniques to approximate the reality of lights, shadows, textures, and materials.  
If you want a raytracer stay tuned for the sequel FINAL DESTINATION coming soon to theaters near you.  
  
# CREDITS  
  
Everything in here is strictly based on the book Mathematics for 3D Game Programming and Computer Graphics (Third Edition) by Eric Lengyel. It will be dubbed as the "Skeleton Book" because of its cover. VSC Verified bears no intentional relationship to any other graphics engine out there. All files were written primarily by NormalExisting using the Skeleton Book as basis and inspiration.
  
# DEDICATIONS  
  
Etienne Vouga (Graphics Professor at UT Austin, some of his teachings influences this work)  
Dave Shreiner (Author of the Official OpenGL Guide)  
Coela Canth (Creator of another rendering engine actually a raytracer)  

VSC Verified is a community-made level created and completed ("verified") by player nSwish in the game Geometry Dash, intended to be a joke but also happens to be extremely challenging. It uses the song "Haven't Met You Yet" by Michael Buble as the background music, and requires several perfectly timed inputs to various rhythms presented by the song.  
  
# HOW TO RECREATE THIS  
  
Follow the Skeleton book until the end of chapter 5. Some linear algebra background is recommended.  
After chapter 5 we make the actual renderer that has the basic rendering abilities of single-color triangles and meshes under a directional light.  
And then after that we go into the more juicy stuff such as texturing, clipping, visibility culling, and the like.  
  
There is a document named `NOTES.md` that holds notes that might be useful.

![Example!](VIDEO.gif?raw=true)