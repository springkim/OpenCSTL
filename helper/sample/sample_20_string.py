import uuid


def main():
    s = str(uuid.uuid4())

    sub = s[2:2 + 8]
    print(sub)

    datas = s.split("-")
    for d in datas:
        print(d)

    new_str = s.replace("-", "==")
    print(new_str)


if __name__ == "__main__":
    main()  
