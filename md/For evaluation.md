
#### Test Nick and Username lenght upon registration

1. Firtsly open irssi:

```
/set nick <new_nick>
/set user_name <new username>
```

2. Exit irssi and reconnect. Now your nick and username are changed

Example of name with 19 characters:
```
Christophe-Alexandre
```

-> name should be silently truncated to 16 characters
-> username should be silently truncated to 9 characters
#### Test topic lenght

```
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed malesuada ultricies arcu sit amet viverra. Vivamus eleifend elit ut ligula blandit tempor. Nullam nec velit non lacus eleifend vulputate. Integer id metus non sapien placerat efficitur. Phasellus mattis ligula nec magna dignissim, vel molestie ipsum convallis. Vestibulum volutpat suscipit erat, non dictum velit vehicula eu. Nullam pulvinar, felis a vestibulum iaculis, nisi erat malesuada mi, nec sollicitudin nunc magna ut felis. Sed et dolor a erat condimentum eleifend nec eu sapien. Morbi id sapien at libero auctor venenatis in nec felis. Vestibulum nec nulla quis dui scelerisque fringilla. Donec lobortis, nulla eget egestas vehicula, arcu elit tincidunt lorem, a ullamcorper arcu velit id leo. Ut quis commodo nunc. Donec eu velit ut felis tempor tincidunt. Ut vestibulum sem id felis malesuada, vel dictum quam tempor. Nam non justo vitae nulla porttitor suscipit. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Duis ultrices felis nec risus varius auctor. Nullam ac justo eget sapien sodales malesuada. Curabitur commodo libero eget libero lacinia, vitae ultrices nibh aliquam. Nam vitae arcu sit amet risus gravida commodo. Donec faucibus turpis eu tincidunt scelerisque. Vestibulum tempor, ipsum vel egestas porttitor, libero libero sollicitudin eros, a congue tortor nisi vitae orci. Duis tincidunt ipsum nec nisi vehicula, nec convallis urna ultricies. Aenean ultrices elit sit amet dolor luctus, et vulputate odio sagittis. Integer nec magna ut libero consequat vestibulum a eu velit. Vivamus ac volutpat risus, in tempor libero. Proin eget efficitur justo. Nullam sit amet nisi vitae velit rutrum dictum. Fusce commodo lorem eu libero congue, vel commodo metus ultricies. Sed ultricies vestibulum orci. Maecenas sagittis volutpat aliquam. Sed eu metus tincidunt, venenatis est eu, consequat purus. Duis fringilla massa at nunc hendrerit congue. Integer at diam sed ex elementum varius sed ut ante. Curabitur non purus ac enim tempor bibendum. Donec in consequat ipsum. Sed convallis arcu nisi, vel sodales magna congue sed. Phasellus vel arcu lobortis, venenatis lacus a, luctus ipsum. Morbi auctor faucibus erat, at tristique nulla dapibus sed. Sed eget ipsum ac libero aliquet dapibus nec nec turpis. In hac habitasse platea dictumst. Cras bibendum semper elit, vel suscipit nisl vehicula vel. Vestibulum vitae mauris vitae justo malesuada convallis. Integer non augue ultricies, cursus odio vel, sollicitudin metus. Nulla nec fermentum dui. Suspendisse gravida mauris eu sem dapibus, ut consectetur metus tempus. Nulla nec enim nec magna vehicula malesuada. Duis tincidunt diam vel nibh iaculis, quis tincidunt orci eleifend. Mauris vitae nisl tempor, varius sem eu, consequat lectus. Duis varius purus ac neque vehicula, nec sollicitudin justo ultricies. Fusce id nibh placerat, suscipit velit nec, tempus dui. Ut pellentesque nulla ut ligula elementum varius. Vivamus in odio vitae lacus varius facilisis. Suspendisse ultricies, lectus eget vestibulum accumsan, quam tortor consequat elit, sed auctor nisl nisl et odio. Nulla facilisi. Integer a efficitur justo. Aliquam non suscipit leo. Nullam pretium sapien non bibendum iaculis. Ut ultrices consequat nulla, vel volutpat dolor sagittis at. Curabitur ac libero ac libero tristique consequat. Proin a ligula libero. Integer ac erat nec nisi accumsan rhoncus ut vitae nulla. Nulla facilisi. Nulla convallis ex id erat vestibulum, eu volutpat neque dap
```

#### Test channel name

Example of channel name with 54 characters:
```
The-great-and-awesome-channel-with-a-very-long-name
```
-> should send illegal channel name error