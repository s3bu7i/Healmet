import smtplib

def send_email(sender_email, sender_password, recipient_email, subject, message):
    # SMTP server configuration
    smtp_server = 'smtp.example.com'  # Replace with your SMTP server address
    smtp_port = 587  # Replace with your SMTP server port
    login_email = sender_email
    login_password = sender_password

    # Email message configuration
    email_subject = subject
    email_message = f"Subject: {email_subject}\n\n{message}"

    try:
        # Establish a connection to the SMTP server
        server = smtplib.SMTP(smtp_server, smtp_port)
        server.starttls()

        # Login to the SMTP server
        server.login(login_email, login_password)

        # Send the email
        server.sendmail(sender_email, recipient_email, email_message)
        print(f"Email sent to {recipient_email}.")

    except Exception as e:
        print(f"An error occurred while sending the email: {str(e)}")

    finally:
        # Close the connection to the SMTP server
        server.quit()

# Example usage
sender_email = 'your_email@example.com'  # Replace with your email address
sender_password = 'your_email_password'  # Replace with your email password
recipient_email = 'recipient@example.com'  # Replace with the recipient's email address
subject = 'Test Email'
message = 'Hello, this is a test email!'

send_email(sender_email, sender_password, recipient_email, subject, message)
