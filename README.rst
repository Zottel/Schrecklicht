##############
 Schrecklicht
##############

Schrecklicht is a small light that is afraid of loud noises.
The intention is to mimic cartoon-like fright and disappearance of animals
when something loud approaches or happens.

The implementation is as simple as possible and uses an ATTiny25, one of the
cheapest DIP microcontrollers I could program at the time.
Noise detection is done with one of the ADCs, with two rolling averages with
different coefficients to determine if the short-term noise level
is significantly above the long-term noise level.

In hardware, the signal for the ADC came out of an operational amplifier which
amplified the microphone input to usable levels.

This project was hacked together long before I took the audio signal processing
courses at university. It should not be used as more than inspiration nowadays.
