function Pass(digit) {
    document.getElementById("pass_input").value.length == 4 ? "" : document.getElementById("pass_input").value += digit;
}

function deleteLastChar() {
    let password = document.getElementById("pass_input");
    let length = password.value.length;
    password.value = password.value.substring(0, length - 1);
}

function submitPass() {
    let passwordVal = document.getElementById("pass_input").value;
    if (passwordVal == 1122) {
        document.getElementById("myLoader").style.opacity = "1";
        setTimeout(() => {
            window.location.href = "home.html";
        }, 1500);

    }
    else {
        Swal.fire({
            icon: 'error',
            title: 'Oops...',
            text: 'Giriş parolunuz yanlışdır !',
        })
        document.getElementById("pass_input").value = "";
    }
}

const checkbox = document.getElementById('myCheckbox')

if (checkbox) {
    checkbox.addEventListener('change', (event) => {
        if (event.currentTarget.checked) {
            document.getElementById("wrapperDescript").innerHTML = "Your GPS is active."
            document.getElementById("descriptMapMy").style.opacity = "0";
            document.getElementById("myIframe").style.opacity = "0.7";
        } else {
            document.getElementById("wrapperDescript").innerHTML = "Your GPS is not active."
            document.getElementById("descriptMapMy").style.opacity = "1";
            document.getElementById("myIframe").style.opacity = "0";
        }
    })
}
