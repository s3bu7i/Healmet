from twilio.rest import Client

# Twilio account SID and auth token
account_sid = 'AC58534176e7a40d44350c4a3b10658f34'
auth_token = '8f0024ac077f0a458952c2cc289fe600'

# Emergency contact number
emergency_contact = '+994508816613'  # Replace with the emergency contact number

def make_emergency_call():
    client = Client(account_sid, auth_token)

    # Send SMS with emergency information
    message = client.messages.create(
        body='Emergency: !',
        from_='+13613262752',  # Replace with your Twilio phone number
        to=emergency_contact
    )

    print(f"SMS sent to {emergency_contact}.")

    # Make a call to the emergency contact
    call = client.calls.create(
        twiml='<Response><Say>Emergency: Stop Detected!</Say></Response>',
        from_='+13613262752',  # Replace with your Twilio phone number
        to=emergency_contact
    )

    print(f"Call made to {emergency_contact}.")

# Call the function to make the emergency call
make_emergency_call()
