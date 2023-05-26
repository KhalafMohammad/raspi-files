# import RPi.GPIO as GPIO
# import time
# GPIO.setmode(GPIO.BCM)
# GPIO.setup(16, GPIO.IN)
# a =3

# while a>1:

#     button_state = GPIO.input(16)
#     # print("Content-type:text/plain\n\n")
#     print(button_state)
#     time.sleep(1)



import RPi.GPIO as GPIO

try:
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(16, GPIO.IN)
    button_state = GPIO.input(16)
    print(button_state)
except Exception as e:
    print(f"Error: {e}")
finally:
    GPIO.cleanup()


