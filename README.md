### Notice
Starting October 12th, 2023 GitHub is enforcing mandatory [two-factor authentication](https://github.blog/2023-03-09-raising-the-bar-for-software-security-github-2fa-begins-march-13/) on my account.  
I'm not going to comply and move all my activity to GitLab instead.  
Any future updates / releases will be available at: [https://gitlab.com/gitendo/fnt2bmp](https://gitlab.com/gitendo/fnt2bmp)  
Thanks and see you there!
___

# fnt2bmp
If you're old enough you might remember DOS utilities like Font Edit or Font Mania which were designed for editing EGA and VGA fonts on PC. This software is obsolete nowadays but fonts still might be usable on different tile based platforms which are lacking pixel artists ie. Gameboy. ;) Since checking each font in DosBox would be cumbersome I wrote this small utility that converts .fnt files into bitmaps. This helps sorting, previewing and further conversion.

### Input
- headerless .fnt files, usually 768, 2048 or 4096 bytes long
- .com files made with Font Mania and from PC Magazine / Michael J. Mefford

Probably some other headers exist but I haven't come across them yet.

### Options
`fnt2bmp [options] file[.fnt]`
```
h# - font height in px (8 - default, 14 or 16)
w# - bitmap width (must be multiple of 8 or will be rounded up)
```
Most .fnt files use 8\*8px per character but you can also find 8\*16 or 8\*14 fonts. First convert without any options and check the bitmap. Then add `-h` param if neccessary. You can also specify bitmap width, where `-w8` will produce all chars in single vertical and `-w2560` horizontal colum. By default it's 256 px.

### Bugs
Let me know.
