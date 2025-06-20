raspberry pi zero 2w , debian, bash send an email when an ssh login happens


Great question.

In the example I gave for the `~/.msmtprc` config:

```ini
user           youraddress@gmail.com
password       yourpassword
```

this line:

```ini
password       yourpassword
```

**stores your Gmail (or SMTP) password in plaintext**, which is **not safe**, especially on shared or internet-connected systems.

---

### 🔐 Safer Alternative: `passwordeval`

Instead of hardcoding your password, you can use:

```ini
passwordeval   "gpg --quiet --for-your-eyes-only --no-tty -d ~/.mailpass.gpg"
```

This tells `msmtp` to **run a command to retrieve the password** at runtime.

---

### ✅ How to set this up

1. **Create an encrypted password file using GPG:**

```bash
echo "your-smtp-password" | gpg --symmetric --cipher-algo AES256 -o ~/.mailpass.gpg
```

You will be asked to enter a passphrase to protect it.

2. **Edit `~/.msmtprc`:**

```ini
passwordeval   "gpg --quiet --for-your-eyes-only --no-tty -d ~/.mailpass.gpg"
```

3. **Set permissions:**

```bash
chmod 600 ~/.mailpass.gpg
```

4. When `msmtp` sends mail, it will decrypt the password on the fly.

---

### 🧠 Summary

* `password`: stores the password directly (not recommended).
* `passwordeval`: safer, uses a command to fetch/decrypt the password at runtime.

Would you like help setting up GPG for this purpose?




