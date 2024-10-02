import os
from mutagen.mp3 import MP3
from mutagen.id3 import ID3, APIC
from PIL import Image
from io import BytesIO
import sys

def extract_cover(file_path):
    try:
        if os.path.exists("cover_image.jpg"):
            os.remove("cover_image.jpg")
            print("Existing cover_image.jpg deleted.")

        audio = MP3(file_path, ID3=ID3)
        for tag in audio.tags.values():
            if isinstance(tag, APIC):
                image_data = tag.data
                image = Image.open(BytesIO(image_data))
                image.save("cover_image.jpg")
                print("Cover image saved to cover_image.jpg")
                return
        print("No APIC frame found")
    except Exception as e:
        print(f"Error: {str(e)}")

if __name__ == "__main__":
    mp3_file_path = sys.argv[1]
    print("Start saving cover image...")
    extract_cover(mp3_file_path)

